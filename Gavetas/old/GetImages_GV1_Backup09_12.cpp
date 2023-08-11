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

using namespace cv;
using namespace std;

HIDS hCam = 0;      // 0 for the next available camera. 1-254 to access by ID
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
	char *path="/home/nvidia/Downloads/Camera/uEye_SimpleSingleGrab";
	int *contEncoder;
	int ProximoRegistro = 0; //Contagem de pulsos do encoder para próxima captura
	
	if((shmidMemoria=shmget(ftok(path,(key_t)KEYEncoder),sizeof(int*),IPC_CREAT|SHM_R|SHM_W))==-1){
		printf("\nErro na criacao do segmento de memoria...\n");
		exit(-1);
	}
	contEncoder=(int*)shmat(shmidMemoria,0,0);
	*contEncoder = 0;
	
	// ========== Matrizes de calibração do sistema ==========
	cv::Mat cameraMatrix = (cv::Mat1d(3, 3) << 	889.8558214627022, 		0, 						730.9657076856182,
												0, 						889.5253507605797, 		512.791593927118,
												0, 						0, 						1);
												
	cv::Mat distCoeffs = (cv::Mat1d(1, 5) << 	-0.3182300709164855, 	0.132980716319103, 		0.002227390313509965, 
												-0.0004478701833075429,	-0.02919535214796576);
	
	// ========== Inicializa conexão e driver da câmera ==========
    is_InitCamera(&hCam, NULL);
	
	// ========== Define região de captura da imagem ==========
    int ImgWidth = 1448; //Max 1448
    int ImgHeight = 100; //Max 1086
    IS_RECT rectAOI;
	rectAOI.s32Width = ImgWidth;
    rectAOI.s32Height = ImgHeight;
    rectAOI.s32X = 0;
    rectAOI.s32Y = 694;
    is_AOI(hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    is_AOI(hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    cout << "Width: " << rectAOI.s32Width << " Height: " << rectAOI.s32Height << " X: " << rectAOI.s32X << " Y: " << rectAOI.s32Y << endl;

	// ========== Seta e imprime novo FPS ==========
	is_SetFrameRate(hCam, 240, &newFPS); //Ajuste do FPS (newFPS recebe o número de FPS atualizado)
	cout << "FPS: " << newFPS << endl;
    
    // ========== Seta tempo de esposição ==========
    double shutter=1; //Tempo de exposição em ms
    is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, &shutter, sizeof(shutter));
    is_Exposure(hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, &shutter, sizeof(shutter));
    cout << "Tempo de Exposição: " << shutter << " ms" << endl;
    
    // ========== Configura gamma ==========
    double gamma=0.6; //1 é a imagem normal, quanto menor mais claro
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

	// Insere parâmetros para realizar a undistortion da câmera
	cv::Mat map1, map2, map3, map4;
	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), cameraMatrix, cv::Size(1448,1086), CV_16SC2, map3, map4 );
	//Seleciona apenas regiões de interesse das matrizes undistortion geradas
	map1 = map3(cv::Rect(0, 694, 1448, 150)).clone();
	map2 = map4(cv::Rect(0, 694, 1448, 150)).clone();
	
	int cont=0;
	VOID* pMem_b;			
	cv::Mat img(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
	cv::Mat imgUndistorted;
	cv::Mat tmp(1086, 1448, CV_8UC3), tmp2(1086, 1448, CV_8UC3);
	cv::Mat image[75];
	cv::Mat imageF;
	
	do{		
		if (*contEncoder>=ProximoRegistro){
			
			cout << cont << endl;
			
			is_FreezeVideo(hCam, IS_WAIT);
			is_GetImageMem(hCam, &pMem_b);
			cv::Mat img(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
			img.copyTo(tmp(cv::Rect(0, 694, img.cols, img.rows)));
			
			cv::remap(tmp, tmp2, map1, map2, 1, 0);
			
			cont++;
			image[cont] = tmp2(cv::Rect(0, 60, 1448, 21)).clone();
			cv::waitKey(5);
	
			if (ProximoRegistro == 0)
				ProximoRegistro = 130;
			else
				ProximoRegistro = ProximoRegistro+60;
		}
	}while(cont<70);
		
	imageF = image[1].clone();
	for (int cont2=2; cont2<=cont; cont2++)
		vconcat(image[cont2], imageF, imageF);

	cv::namedWindow("Pre-filtro", CV_WINDOW_NORMAL);
	cv::imshow("Pre-filtro",imageF);
	
	// Aplica correção de gamma
	LUT(imageF, lut_matrix, imageF);

	// Exibe e sava imagem
	imwrite("Final.jpg", imageF);
	cv::namedWindow("Image", CV_WINDOW_NORMAL);
	cv::imshow("Image",imageF);
	cv::waitKey();

	// Fecha todos os dispositivos e janelas
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);
    is_ExitCamera(hCam);
	cv::destroyAllWindows();
	
	cout << "Encoder: " << *contEncoder << endl;

    return 0;
}
