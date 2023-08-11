#include <iostream>
#include <ueye.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string.h>
#include <sstream>
#include <math.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <vector>
#include <cmath>
#include "cppflow/cppflow.h"
#include "tensorflow/c/c_api.h"

using namespace cv;
using namespace std;

HIDS hCam = 2;      // 0 for the next available camera. 1-254 to access by ID
SENSORINFO sInfo;
HWND hWndDisplay;
SENSORSCALERINFO sScaleInfo;

char* pcImageMemory;
int DisplayWidth, DisplayHeight;
double newFPS;

//Endereços de memória compartilhada
#define KEYEncoder 9902
#define KEYImagemCam2OK 9904

int main()
{
	// ========== Memória compartilhada ==========
	int shmidMemoria, shmidImagemCam2;
	char *path="/home/tramontina/Downloads/Gavetas";
	int *contEncoder, *ImagemCam2OK;
	int ProximoRegistro = 0; //Contagem de pulsos do encoder para próxima captura
	
	if((shmidMemoria=shmget(ftok(path,(key_t)KEYEncoder),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	contEncoder=(int*)shmat(shmidMemoria,0,0);
	*contEncoder = 0;

    if((shmidImagemCam2=shmget(ftok(path,(key_t)KEYImagemCam2OK),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	ImagemCam2OK=(int*)shmat(shmidImagemCam2,0,0);
    *ImagemCam2OK = 0;
	
	// ========== Inicializa conexão e driver da câmera ==========
    is_InitCamera(&hCam, NULL);
	
	// ========== Define região de captura da imagem ==========
    int ImgWidth = 432; //Max 1448
    int regiaoScan = 2;
    int ImgHeight = 4; //Max 1086 //100 obs. deve resultar em um número par
    int OffsetX = 490;
    int OffsetY = 645;
    
    IS_RECT rectAOI;
	rectAOI.s32Width = ImgWidth;
    rectAOI.s32Height = ImgHeight;
    rectAOI.s32X = OffsetX;
    rectAOI.s32Y = OffsetY;
    is_AOI(hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    is_AOI(hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    cout << "Width: " << rectAOI.s32Width << " Height: " << rectAOI.s32Height << " X: " << rectAOI.s32X << " Y: " << rectAOI.s32Y << endl;

    is_SetSubSampling(hCam, IS_SUBSAMPLING_2X_VERTICAL); //IS_SUBSAMPLING_2X_VERTICAL | IS_SUBSAMPLING_2X_HORIZONTAL
    cout << "Subsampling: " << is_SetSubSampling(hCam, IS_GET_SUBSAMPLING) << endl;

	// ========== Seta tempo de esposição ==========
    double shutter=1.0; //Tempo de exposição em ms
    is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, &shutter, sizeof(shutter));
    is_Exposure(hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, &shutter, sizeof(shutter));
    cout << "Tempo de Exposição: " << shutter << " ms" << endl;
	
	// ========== Seta e imprime novo FPS ==========
	//Obs. Insere o maior valor possível de acordo com o tempo de exposição
	is_SetFrameRate(hCam, 1000/shutter, &newFPS); //Ajuste do FPS (newFPS recebe o número de FPS atualizado)
	cout << "FPS: " << newFPS << endl;
    
    // ========== Configura gamma ==========
    double gamma=0.5; //1 é a imagem normal, quanto menor mais claro
	Mat lut_matrix(1, 256, CV_8U);
	uchar * ptr = lut_matrix.ptr();
	for(int i=0; i<256; i++)
		ptr[i] = saturate_cast<uchar>(pow(i/255.0, gamma)*255.0);
    
    // ========== Carrega informações da câmera e define região para armazenamento da imagem ==========
    is_GetSensorInfo(hCam, &sInfo);
    DisplayWidth = ImgWidth;//sInfo.nMaxWidth;
    DisplayHeight = ImgHeight;//sInfo.nMaxHeight;
    cout << "Resolução: " << DisplayWidth << "x" << DisplayHeight << endl;
	
	// ========== Procura e seta o modo de cor da câmera ==========
    int nColorMode;
    int nBitsPerPixel = 24;

    if (sInfo.nColorMode == IS_COLORMODE_BAYER)
    {
        // For color camera models use RGB24 mode
        nColorMode = IS_CM_BGR8_PACKED;
        nBitsPerPixel = 24;
    }
    else if (sInfo.nColorMode == IS_COLORMODE_CBYCRY)
    {
        // For CBYCRY camera models use RGB32 mode
        nColorMode = IS_CM_BGRA8_PACKED;
        nBitsPerPixel = 32;
    }
    else
    {
        // For monochrome camera models use Y8 mode
        nColorMode = IS_CM_MONO8;
        nBitsPerPixel = 24;
    }
    
	is_SetColorMode(hCam,nColorMode);
	
	// ========== Aquisição de imagens da câmera ==========
	int nMemoryId;
	
    // Assigns a memory for the image and sets it active
    is_AllocImageMem(hCam, DisplayWidth, DisplayHeight, nBitsPerPixel, &pcImageMemory, &nMemoryId);
    is_SetImageMem(hCam, pcImageMemory, nMemoryId);
    
    int cont=0;
	int i=0;
	VOID* pMem_b;
	cv::Mat img1[1000];
	cv::Mat imgUndistorted;
	cv::Mat tmp1a(1086, 1448, CV_8UC3), tmp1b(1086, 1448, CV_8UC3);
	cv::Mat image1F, image2F;
	cv::Mat img1aux;

    while(1){
        cont = 0;
        ProximoRegistro = 1; //Contagem de pulsos do encoder para próxima captura
        while(*contEncoder>0);
        do{
            if (*contEncoder>=ProximoRegistro){
                //cout << "Cam 1 start - Contador: " << ProximoRegistro << " Encoder 1: " << *contEncoder << endl;
                is_FreezeVideo(hCam, IS_WAIT);
                is_GetImageMem(hCam, &pMem_b);
                cv::Mat img1aux(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
                cv::resize(img1aux, img1[cont], cv::Size(), 0.5, 0.5);
                //img1[cont] = img1aux.clone();
                ProximoRegistro = ProximoRegistro+15;
                cont++;
                //cout << "Cam 1 end  - Encoder: " << *contEncoder << " Total: " << *contEncoder-ProximoRegistro+32 << endl;
                //cout << "Contador: " << *contEncoder << "\nCont: " << cont << endl;
            }
        }while(*contEncoder!=0 || cont==0);
        
        //Concatena região de interesse das imagens obtidas anteriormente em uma única imagem
        image1F = img1[1](Rect(0, 0, ImgWidth/2, regiaoScan/2)).clone();
        for (int cont2=2; cont2<cont-1; cont2++){
            vconcat(img1[cont2](Rect(0, 0, ImgWidth/2, regiaoScan/2)), image1F, image1F);
        }
                
        // Aplica correção de gamma
        LUT(image1F, lut_matrix, image1F);
		// Exibe e sava imagem
        cv::Mat resized;
        resize(image1F, resized, Size(ImgWidth/2, 480/2), 0, 0, INTER_CUBIC);
		std::string name = "Cam2_" + std::to_string(i) + ".png"; //Define endereço das imagens
		imwrite(name, resized);
		cv::namedWindow("Image", CV_WINDOW_NORMAL);
		cv::imshow("Image",resized);
		cv::waitKey(1);
		cout << "Frame capturado: " << i << endl;
		i++;
    }

	// ========== Recorta imagem para treinamento da rede neural ==========
    /*
    int x,y,width,height;
	
	//Ferramenta 1
    y = 820/2;
    height = 1100/2-y;
    x = 85/2;
    width = 205/2-x;
	cv::Mat tool1 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool1, tool1, cv::Size(), 0.5, 0.5);
	imwrite("Tool1.png", tool1);

	//Ferramenta 2
    y = 830/2;
    height = 1100/2-y;
    x = 230/2;
    width = 350/2-x;
	cv::Mat tool2 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool2, tool2, cv::Size(), 0.5, 0.5);
	imwrite("Tool2.png", tool2);

	//Ferramenta 3
    y = 830/2;
    height = 1100/2-y;
    x = 385/2;
    width = 515/2-x;
	cv::Mat tool3 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool3, tool3, cv::Size(), 0.5, 0.5);
	imwrite("Tool3.png", tool3);

	//Ferramenta 4
    y = 200/2;
    height = 850/2-y;
    x = 90/2;
    width = 200/2-x;
	cv::Mat tool4 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool4, tool4, cv::Size(), 0.5, 0.5);
	imwrite("Tool4.png", tool4);

	//Ferramenta 5
    y = 185/2;
    height = 855/2-y;
    x = 235/2;
    width = 355/2-x;
	cv::Mat tool5 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool5, tool5, cv::Size(), 0.5, 0.5);
	imwrite("Tool5.png", tool5);

	//Ferramenta 6
    y = 185/2;
    height = 855/2-y;
    x = 390/2;
    width = 510/2-x;
	cv::Mat tool6 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool6, tool6, cv::Size(), 0.5, 0.5);
	imwrite("Tool6.png", tool6);

	//Ferramenta 7
    y = 284/2;
    height = 1000/2-y;
    x = 600/2;
    width = 720/2-x;
	cv::Mat tool7 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool7, tool7, cv::Size(), 0.5, 0.5);
	imwrite("Tool7.png", tool7);

	//Ferramenta 8
    y = 320/2;
    height = 1000/2-y;
    x = 740/2;
    width = 860/2-x;
	cv::Mat tool8 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool8, tool8, cv::Size(), 0.5, 0.5);
	imwrite("Tool8.png", tool8);*/

    *ImagemCam2OK = 1;
    
	// ========== Fecha todos os dispositivos e janelas ==========
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);
    is_ExitCamera(hCam);
	cv::destroyAllWindows();
	
    return 0;
}
