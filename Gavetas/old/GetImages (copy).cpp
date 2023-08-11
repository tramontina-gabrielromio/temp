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

#include <stdio.h>
#include <time.h> 
#include <sys/time.h>
#include <sys/resource.h>

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
	/*//Testes time
	struct rusage buf;
	//for (int i=0; i<50000000; i++){}
	getrusage(RUSAGE_SELF, &buf);
	cout << "seconds: " << buf.ru_utime.tv_sec << " mseconds: " << buf.ru_utime.tv_usec/1000 << endl;*/
	
	// ========== Memória compartilhada ==========
	int shmidMemoria;
	char *path="/home/nvidia/Downloads/Camera/uEye_SimpleSingleGrab";
	int *contEncoder;
	int ProximoRegistro = 50; //Contagem de pulsos do encoder para próxima captura
	char numstr[10];
	
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
    int nRet2 = is_AOI(hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));
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
	cv::Mat map1, map2;
	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), cameraMatrix, cv::Size(1448,1086), CV_16SC2, map1, map2 );
	
	int cont=0;
	VOID* pMem_b;			
	cv::Mat img(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
	cv::Mat imgUndistorted;
	cv::Mat tmp(1086, 1448, CV_8UC3);
	cv::Mat image[70];
	cv::Mat imageF;
	
	cv::namedWindow("Final", CV_WINDOW_NORMAL);
	
	do{		
		if (*contEncoder>=ProximoRegistro){
			cout << cont << endl;
			// Registra um frame da câmera
			is_FreezeVideo(hCam, IS_WAIT);
			is_GetImageMem(hCam, &pMem_b);
			
			cv::Mat img(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
			img.copyTo(tmp(cv::Rect(0, 669, img.cols, img.rows)));
			
			cv::remap(tmp, img, map1, map2, 1, 0);
			cont++;
			image[cont] = img(cv::Rect(0, 739, 1448, 21)).clone();
			
			cv::imshow("Final",image[cont]);
			cv::waitKey(5);
	
			if (ProximoRegistro == 0)
				ProximoRegistro = 90;
			else
				ProximoRegistro = ProximoRegistro+40;
		}
	}while(cont<65);
		
	imageF = image[1].clone();
	for (int cont2=2; cont2<=cont; cont2++)
		vconcat(image[cont2], imageF, imageF);

	imwrite("Final.jpg", imageF);
	cv::namedWindow("Image", CV_WINDOW_NORMAL);
	cv::imshow("Image",imageF);
	cv::waitKey();

    // Releases an image memory that was allocated
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);

    // Disables the hCam camera handle and releases the data structures and memory areas taken up by the uEye camera
    is_ExitCamera(hCam);
	
	// Fecha todas as janelas
	cv::destroyAllWindows();

    return 0;
}
