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

#define KEYEncoder 9902 //Endereço de memória compartilhada

int main()
{
	// ========== Memória compartilhada ==========
	int shmidMemoria;
	char *path="/home/tramontina/Downloads/Gavetas";
	int *contEncoder;
	int ProximoRegistro = 0; //Contagem de pulsos do encoder para próxima captura
	
	if((shmidMemoria=shmget(ftok(path,(key_t)KEYEncoder),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	contEncoder=(int*)shmat(shmidMemoria,0,0);
	*contEncoder = 0;
	
	// ========== Inicializa conexão e driver da câmera ==========
    is_InitCamera(&hCam, NULL);
	
	// ========== Define região de captura da imagem ==========
    int ImgWidth = 880; //Max 1448
    int regiaoScan = 2;
    int ImgHeight = 4; //Max 1086 //100 obs. deve resultar em um número par
    int OffsetX = 300;
    int OffsetY = 760;
    
    IS_RECT rectAOI;
	rectAOI.s32Width = ImgWidth;
    rectAOI.s32Height = ImgHeight;
    rectAOI.s32X = OffsetX;
    rectAOI.s32Y = OffsetY;
    is_AOI(hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    is_AOI(hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    cout << "Width: " << rectAOI.s32Width << " Height: " << rectAOI.s32Height << " X: " << rectAOI.s32X << " Y: " << rectAOI.s32Y << endl;

	// ========== Seta tempo de esposição ==========
    double shutter=0.7; //Tempo de exposição em ms
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
	VOID* pMem_b;
	cv::Mat img1[1000];
	cv::Mat imgUndistorted;
	cv::Mat tmp1a(1086, 1448, CV_8UC3), tmp1b(1086, 1448, CV_8UC3);
	cv::Mat image1F, image2F;
	cv::Mat img1aux;

	cont = 0;
	ProximoRegistro = 3; //Contagem de pulsos do encoder para próxima captura
	while(*contEncoder>0);
	do{
		if (*contEncoder>=ProximoRegistro){
			//cout << "Cam 1 start - Contador: " << ProximoRegistro << " Encoder 1: " << *contEncoder << endl;
			is_FreezeVideo(hCam, IS_WAIT);
			is_GetImageMem(hCam, &pMem_b);
			cv::Mat img1aux(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
			img1[cont] = img1aux.clone();
			ProximoRegistro = ProximoRegistro+6;
			cont++;
			//cout << "Cam 1 end  - Encoder: " << *contEncoder << " Total: " << *contEncoder-ProximoRegistro+32 << endl;
            //cout << "Contador: " << *contEncoder << "\nCont: " << cont << endl;
		}
	}while(*contEncoder!=0 || cont==0);

	//Concatena região de interesse das imagens obtidas anteriormente em uma única imagem
	image1F = img1[1](Rect(0, 0, ImgWidth, regiaoScan)).clone();
	for (int cont2=2; cont2<cont-1; cont2++){
		vconcat(img1[cont2](Rect(0, 0, ImgWidth, regiaoScan)), image1F, image1F);
	}
            
    // Aplica correção de gamma
    LUT(image1F, lut_matrix, image1F);

    // Salva imagem
	std::string name = "GV2_cam1.png"; //Define endereço das imagens
	imwrite(name, image1F);

	cv::Mat resized;
	resize(image1F, resized, Size(ImgWidth, 1100), 0, 0, INTER_CUBIC);
	name = "GV2_cam1_resized.png";
	imwrite(name, resized);


    // ========== Recorta imagem para treinamento da rede neural ==========

    int x,y,width,height;

	//Ferramenta 9
    y = 250;
    height = 1000-y;
    x = 20;
    width = 140-x;
	cv::Mat tool9 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool9, tool9, cv::Size(), 0.5, 0.5);
	imwrite("Tool9.png", tool9);

	//Ponteira 9
    y = 880;
    height = 1000-y;
    x = 65;
    width = 105-x;
	cv::Mat pont9 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont9.png", pont9);

	//Ferramenta 10
    y = 180;
    height = 1010-y;
    x = 155;
    width = 285-x;
	cv::Mat tool10 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool10, tool10, cv::Size(), 0.5, 0.5);
	imwrite("Tool10.png", tool10);

	//Ponteira 10
    y = 860;
    height = 1000-y;
    x = 200;
    width = 250-x;
	cv::Mat pont10 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont10.png", pont10);

	//Ferramenta 11
    y = 170;
    height = 1020-y;
    x = 305;
    width = 435-x;
	cv::Mat tool11 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool11, tool11, cv::Size(), 0.5, 0.5);
	imwrite("Tool11.png", tool11);

	//Ponteira 11
    y = 890;
    height = 1000-y;
    x = 350;
    width = 400-x;
	cv::Mat pont11 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont11.png", pont11);

	//Ferramenta 12
    y = 170;
    height = 1020-y;
    x = 460;
    width = 600-x;
	cv::Mat tool12 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool12, tool12, cv::Size(), 0.5, 0.5);
	imwrite("Tool12.png", tool12);

	//Ponteira 12
    y = 840;
    height = 1000-y;
    x = 500;
    width = 550-x;
	cv::Mat pont12 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont12.png", pont12);

	//Ferramenta 13
    y = 170;
    height = 1010-y;
    x = 615;
    width = 745-x;
	cv::Mat tool13 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool13, tool13, cv::Size(), 0.5, 0.5);
	imwrite("Tool13.png", tool13);

	//Ponteira 13
    y = 870;
    height = 1000-y;
    x = 645;
    width = 695-x;
	cv::Mat pont13 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont13.png", pont13);


	// ========== Fecha todos os dispositivos e janelas ==========
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);
    is_ExitCamera(hCam);
	cv::destroyAllWindows();
	
    return 0;
}
