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

HIDS hCam = 1;      // 0 for the next available camera. 1-254 to access by ID
SENSORINFO sInfo;
HWND hWndDisplay;
SENSORSCALERINFO sScaleInfo;

char* pcImageMemory;
int DisplayWidth, DisplayHeight;
double newFPS;

//Endereço de memória compartilhada
#define KEYEncoder 9902
#define KEYImagemCam1OK 9903

int main()
{
	// ========== Memória compartilhada ==========
	int shmidMemoria, shmidImagemCam1;
	char *path="/home/tramontina/Downloads/Gavetas";
	int *contEncoder, *ImagemCam1OK;
	int ProximoRegistro = 0; //Contagem de pulsos do encoder para próxima captura
	
	if((shmidMemoria=shmget(ftok(path,(key_t)KEYEncoder),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	contEncoder=(int*)shmat(shmidMemoria,0,0);
	*contEncoder = 0;

    if((shmidImagemCam1=shmget(ftok(path,(key_t)KEYImagemCam1OK),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	ImagemCam1OK=(int*)shmat(shmidImagemCam1,0,0);
    *ImagemCam1OK = 0;
	
	// ========== Inicializa conexão e driver da câmera ==========
    is_InitCamera(&hCam, NULL);
	
	// ========== Define região de captura da imagem ==========
    int ImgWidth = 480; //Max 1448
    int regiaoScan = 2;
    int ImgHeight = 4; //Max 1086 //100 obs. deve resultar em um número par
    int OffsetX = 420;
    int OffsetY = 1086-590; //obs. o valor é reduzido de 1086 por que a câmera está invertida verticalmente
    
    IS_RECT rectAOI;
	rectAOI.s32Width = ImgWidth;
    rectAOI.s32Height = ImgHeight;
    rectAOI.s32X = OffsetX;
    rectAOI.s32Y = OffsetY;
    is_AOI(hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    is_AOI(hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    cout << "Width: " << rectAOI.s32Width << " Height: " << rectAOI.s32Height << " X: " << rectAOI.s32X << " Y: " << rectAOI.s32Y << endl;

    is_SetRopEffect(hCam, IS_SET_ROP_MIRROR_UPDOWN, 1, 0); //Inverte imagem verticalmente (apenas para a cam1)
    is_SetRopEffect(hCam, IS_SET_ROP_MIRROR_LEFTRIGHT, 1, 0); //Inverte imagem horizontalmente (apenas para a cam1)
    cout << "Rotacao: " << is_SetRopEffect(hCam, IS_GET_ROP_EFFECT, 1, 0) << endl;

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
        ProximoRegistro = 3; //Contagem de pulsos do encoder para próxima captura
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
		std::string name = "Cam1_" + std::to_string(i) + ".png"; //Define endereço das imagens
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

	//Ferramenta 9
    y = 250/2;
    height = 1000/2-y;
    x = 30/2;
    width = 150/2-x;
	cv::Mat tool9 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool9, tool9, cv::Size(), 0.5, 0.5);
	imwrite("Tool9.png", tool9);

	//Ferramenta 10
    y = 180/2;
    height = 1010/2-y;
    x = 165/2;
    width = 295/2-x;
	cv::Mat tool10 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool10, tool10, cv::Size(), 0.5, 0.5);
	imwrite("Tool10.png", tool10);

	//Ferramenta 11
    y = 170/2;
    height = 1020/2-y;
    x = 315/2;
    width = 445/2-x;
	cv::Mat tool11 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool11, tool11, cv::Size(), 0.5, 0.5);
	imwrite("Tool11.png", tool11);

	//Ferramenta 12
    y = 170/2;
    height = 1020/2-y;
    x = 470/2;
    width = 610/2-x;
	cv::Mat tool12 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool12, tool12, cv::Size(), 0.5, 0.5);
	imwrite("Tool12.png", tool12);

	//Ferramenta 13
    y = 170/2;
    height = 1010/2-y;
    x = 625/2;
    width = 755/2-x;
	cv::Mat tool13 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    //cv::resize(tool13, tool13, cv::Size(), 0.5, 0.5);
	imwrite("Tool13.png", tool13);
    */
    *ImagemCam1OK = 1;

	// ========== Fecha todos os dispositivos e janelas ==========
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);
    is_ExitCamera(hCam);
	cv::destroyAllWindows();
	
    return 0;
}
