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
#include <thread>
#include "cppflow/cppflow.h"
#include "tensorflow/c/c_api.h"

using namespace cv;
using namespace std;

#define KEYEncoder 9902 //Endereço de memória compartilhada

int *contEncoder;
int cam1Return = 0;
int cam2Return = 0;

void taskCam1()
{
	// ========== Matrizes de calibração do sistema ==========
	cv::Mat cameraMatrix = (cv::Mat1d(3, 3) << 	892.1691931981718, 		0, 						724.1965286470058,
												0, 						892.2270082418462, 		586.1900181919949,
												0, 						0, 						1);
												
	cv::Mat distCoeffs = (cv::Mat1d(1, 5) << 	-0.3319552721123589, 	0.1552019675899306, 		-0.001548766723012425, 
												-9.371354036299631e-05,	-0.04069783306301979);
	
	// ========== Inicializa conexão e driver da câmera ==========
	HIDS hCam1 = 1;
	SENSORINFO sInfo;
	HWND hWndDisplay;
	SENSORSCALERINFO sScaleInfo;
	char* pcImageMemory1;
	int DisplayWidth, DisplayHeight;
	double newFPS;
    is_InitCamera(&hCam1, NULL);
	
	// ========== Define região de captura da imagem ==========
    int ImgWidth = 1000; //Max 1448
    int ImgHeight = 80; //Max 1086
    int OffsetX = 200;
    int OffsetY = 700;

    IS_RECT rectAOI;
	rectAOI.s32Width = ImgWidth;
    rectAOI.s32Height = ImgHeight;
    rectAOI.s32X = OffsetX;
    rectAOI.s32Y = OffsetY;
    is_AOI(hCam1, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    is_AOI(hCam1, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    cout << "Width: " << rectAOI.s32Width << " Height: " << rectAOI.s32Height << " X: " << rectAOI.s32X << " Y: " << rectAOI.s32Y << endl;

	// ========== Seta e imprime novo FPS ==========
	is_SetFrameRate(hCam1, 240, &newFPS); //Ajuste do FPS (newFPS recebe o número de FPS atualizado)
	cout << "FPS Cam1: " << newFPS << endl;
    
    // ========== Seta tempo de esposição ==========
    double shutter=1; //Tempo de exposição em ms
    is_Exposure(hCam1, IS_EXPOSURE_CMD_SET_EXPOSURE, &shutter, sizeof(shutter));
    is_Exposure(hCam1, IS_EXPOSURE_CMD_GET_EXPOSURE, &shutter, sizeof(shutter));
    cout << "Tempo de Exposição Cam1: " << shutter << " ms" << endl;
    
    // ========== Configura gamma ==========
    double gamma=0.6; //1 é a imagem normal, quanto menor mais claro
	Mat lut_matrix(1, 256, CV_8U);
	uchar * ptr = lut_matrix.ptr();
	for(int i=0; i<256; i++)
		ptr[i] = saturate_cast<uchar>(pow(i/255.0, gamma)*255.0);
    
    // ========== Carrega informações da câmera e define região para armazenamento da imagem ==========
    is_GetSensorInfo(hCam1, &sInfo);
    DisplayWidth = ImgWidth;//sInfo.nMaxWidth;
    DisplayHeight = ImgHeight;//sInfo.nMaxHeight;
    cout << "Resolução Cam1: " << DisplayWidth << "x" << DisplayHeight << endl;

	// ========== Desabilita filtros e correções de cor ==========
	double ccorMode=0;
    is_SetColorCorrection(hCam1, IS_CCOR_DISABLE, &ccorMode);
	
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
    
	is_SetColorMode(hCam1,nColorMode);
	
	// ========== Aquisição de imagens da câmera ==========
	int nMemoryId1;
	
    // Assigns a memory for the image and sets it active
    is_AllocImageMem(hCam1, DisplayWidth, DisplayHeight, nBitsPerPixel, &pcImageMemory1, &nMemoryId1);
    is_SetImageMem(hCam1, pcImageMemory1, nMemoryId1);

	// Insere parâmetros para realizar a undistortion da câmera
	cv::Mat map1, map2, map3, map4;
	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), cameraMatrix, cv::Size(1448,1086), CV_16SC2, map3, map4 );
	//Seleciona apenas regiões de interesse das matrizes undistortion geradas
	map1 = map3(cv::Rect(0, OffsetY, 1448, 100)).clone();
	map2 = map4(cv::Rect(0, OffsetY, 1448, 100)).clone();
	
	int cont=0;
	VOID* pMem_b1;
	cv::Mat img1[75];
	cv::Mat imgUndistorted;
	cv::Mat tmp1a(1086, 1448, CV_8UC3), tmp1b(1086, 1448, CV_8UC3);
	cv::Mat image1[75], image2[75];
	cv::Mat image1F, image2F;
	cv::Mat img1aux;

	cont = 0;
	int ProximoRegistro = 1; //Contagem de pulsos do encoder para próxima captura
	while(*contEncoder>0);
	do{
		if (*contEncoder>=ProximoRegistro){
			cout << "Cam 1 start - Contador: " << ProximoRegistro << " Encoder 1: " << *contEncoder << endl;
			is_FreezeVideo(hCam1, IS_WAIT);
			is_GetImageMem(hCam1, &pMem_b1);
			cv::Mat img1aux(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b1);
			img1[cont] = img1aux.clone();
			ProximoRegistro = ProximoRegistro+32;
			cont++;
			cout << "Cam 1 end  - Encoder: " << *contEncoder << endl;
		}
	}while(cont<85);
	
	for(int cont2=0; cont2<cont; cont2++){
		img1[cont2].copyTo(tmp1a(cv::Rect(OffsetX, OffsetY, img1[cont2].cols, img1[cont2].rows)));
		cv::remap(tmp1a, tmp1b, map1, map2, 1, 0);
		image1[cont2] = tmp1b(cv::Rect(OffsetX, 24, ImgWidth, 13)).clone();
	}

	image1F = image1[1].clone();
	for (int cont2=2; cont2<cont-1; cont2++){
		vconcat(image1[cont2], image1F, image1F);
		cout << cont2 << endl;
	}
	// Aplica correção de gamma
	LUT(image1F, lut_matrix, image1F);

	// Exibe e salva imagem
	imwrite("1.png", image1F);

    // ========== Fecha a câmera ==========
    is_FreeImageMem(hCam1, pcImageMemory1, nMemoryId1);
    is_ExitCamera(hCam1);

    cam1Return = 1;
}

void taskCam2()
{
    // ========== Matrizes de calibração do sistema ==========
	cv::Mat cameraMatrix = (cv::Mat1d(3, 3) << 	871.5735231986554,                 0,      695.1935909410628,
                                                                0, 872.8234626807015,      582.6862836036451,
                                                                0,                 0,      1);
												
	cv::Mat distCoeffs = (cv::Mat1d(1, 5) << -0.3159324439083396, 0.12663216520726, 0.0001096785038095999, 
                                             -0.001068398630520406, -0.02692827960441731);
	
	// ========== Inicializa conexão e driver da câmera ==========
	HIDS hCam2 = 2;
	SENSORINFO sInfo;
	HWND hWndDisplay;
	SENSORSCALERINFO sScaleInfo;
	char* pcImageMemory2;
	int DisplayWidth, DisplayHeight;
	double newFPS;
	is_InitCamera(&hCam2, NULL);
	
	// ========== Define região de captura da imagem ==========
    int ImgWidth = 1448; //Max 1448
    int ImgHeight = 64; //Max 1086
    IS_RECT rectAOI;
	rectAOI.s32Width = ImgWidth;
    rectAOI.s32Height = ImgHeight;
    rectAOI.s32X = 0;
    rectAOI.s32Y = 724;
    is_AOI(hCam2, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    is_AOI(hCam2, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    cout << "Width: " << rectAOI.s32Width << " Height: " << rectAOI.s32Height << " X: " << rectAOI.s32X << " Y: " << rectAOI.s32Y << endl;

	// ========== Seta e imprime novo FPS ==========
	is_SetFrameRate(hCam2, 240, &newFPS); //Ajuste do FPS (newFPS recebe o número de FPS atualizado)
	cout << "FPS Cam2: " << newFPS << endl;
    
    // ========== Seta tempo de esposição ==========
    double shutter=1.3; //Tempo de exposição em ms
    is_Exposure(hCam2, IS_EXPOSURE_CMD_SET_EXPOSURE, &shutter, sizeof(shutter));
    is_Exposure(hCam2, IS_EXPOSURE_CMD_GET_EXPOSURE, &shutter, sizeof(shutter));
    cout << "Tempo de Exposição Cam2: " << shutter << " ms" << endl;
    
    // ========== Configura gamma ==========
    double gamma=0.6; //1 é a imagem normal, quanto menor mais claro
	Mat lut_matrix(1, 256, CV_8U);
	uchar * ptr = lut_matrix.ptr();
	for(int i=0; i<256; i++)
		ptr[i] = saturate_cast<uchar>(pow(i/255.0, gamma)*255.0);
    
    // ========== Carrega informações da câmera e define região para armazenamento da imagem ==========
	is_GetSensorInfo(hCam2, &sInfo);
    DisplayWidth = ImgWidth;//sInfohWndDisplay.nMaxWidth;
    DisplayHeight = ImgHeight;//sInfo.nMaxHeight;
    cout << "Resolução Cam2: " << DisplayWidth << "x" << DisplayHeight << endl;
	
	// ========== Desabilita filtros e correções de cor ==========
	double ccorMode=0;
    is_SetColorCorrection(hCam2, IS_CCOR_DISABLE, &ccorMode);

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
    
	is_SetColorMode(hCam2,nColorMode);
	
	// ========== Aquisição de imagens da câmera ==========
	int nMemoryId2;
	
    // Assigns a memory for the image and sets it active
    is_AllocImageMem(hCam2, DisplayWidth, DisplayHeight, nBitsPerPixel, &pcImageMemory2, &nMemoryId2);
    is_SetImageMem(hCam2, pcImageMemory2, nMemoryId2);

	// Insere parâmetros para realizar a undistortion da câmera
	cv::Mat map1, map2, map3, map4;
	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), cameraMatrix, cv::Size(1448,1086), CV_16SC2, map3, map4 );
	//Seleciona apenas regiões de interesse das matrizes undistortion geradas
	map1 = map3(cv::Rect(0, 724, 1448, 150)).clone();
	map2 = map4(cv::Rect(0, 724, 1448, 150)).clone();
	
	int cont=0;
	VOID* pMem_b2;
	cv::Mat img2[75];
	cv::Mat imgUndistorted;
	cv::Mat tmp2a(1086, 1448, CV_8UC3), tmp2b(1086, 1448, CV_8UC3);
	cv::Mat image2[75];
	cv::Mat image2F;
	cv::Mat img2aux;
	
	cont = 0;
	int ProximoRegistro = 51; //Contagem de pulsos do encoder para próxima captura
	while(*contEncoder>0);
	do{
		if (*contEncoder>=ProximoRegistro){
			cout << "Cam 2 start - Contador: " << ProximoRegistro << " Encoder: " << *contEncoder << endl;
			is_FreezeVideo(hCam2, IS_WAIT);
			is_GetImageMem(hCam2, &pMem_b2);
			cv::Mat img2aux(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b2);
			img2[cont] = img2aux.clone();
			ProximoRegistro = ProximoRegistro+100;
			cont++;
			cout << "Cam 2 end  - Encoder: " << *contEncoder << endl;
		}
	}while(cont<32);

	for(int cont2=0; cont2<cont; cont2++){
		img2[cont2].copyTo(tmp2a(cv::Rect(0, 724, img2[cont2].cols, img2[cont2].rows)));
		cv::remap(tmp2a, tmp2b, map1, map2, 1, 0);
		image2[cont2] = tmp2b(cv::Rect(0, 32, 1448, 35)).clone();
	}

	image2F = image2[1].clone();
	for (int cont2=2; cont2<cont-1; cont2++){
		vconcat(image2[cont2], image2F, image2F);}
		
	// Aplica correção de gamma
	LUT(image2F, lut_matrix, image2F);

	// Exibe e salva imagem
	imwrite("2.png", image2F);

    // ========== Fecha a câmera ==========
    is_FreeImageMem(hCam2, pcImageMemory2, nMemoryId2);
    is_ExitCamera(hCam2);

    cam2Return = 1;
}

int main()
{
    // ========== Memória compartilhada ==========
	int shmidMemoria;
	char *path="/home/tramontina/Downloads/Gavetas";
	
	if((shmidMemoria=shmget(ftok(path,(key_t)KEYEncoder),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	contEncoder=(int*)shmat(shmidMemoria,0,0);
	*contEncoder = 0;

    thread t1(taskCam1);
    //thread t2(taskCam2);
    t1.detach();
    //t2.detach();
	
	while(cam1Return!=1/* || cam2Return!=1*/){}
		
	// ========== Fecha todas as janelas ==========
	cv::destroyAllWindows();
	
    return 0;
}
