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

HIDS hCam = 2;      // 0 for the next available camera. 1-254 to access by ID
SENSORINFO sInfo;
HWND hWndDisplay;
SENSORSCALERINFO sScaleInfo;

char* pcImageMemory;
int DisplayWidth, DisplayHeight;
double newFPS;

using namespace std;

int main()
{
	// ========== Matrizes de calibração do sistema ==========
	cv::Mat cameraMatrix = (cv::Mat1d(3, 3) << 	871.5735231986554,                 0,      695.1935909410628,
                                                                0, 872.8234626807015,      582.6862836036451,
                                                                0,                 0,      1);
												
	cv::Mat distCoeffs = (cv::Mat1d(1, 5) << -0.3159324439083396, 0.12663216520726, 0.0001096785038095999, 
                                             -0.001068398630520406, -0.02692827960441731);
	
	// ========== Inicializa conexão e driver da câmera ==========
    is_InitCamera(&hCam, NULL);
	
	// ========== Define região de captura da imagem ==========
    int ImgWidth = 1448; //Max 1448
    int ImgHeight = 1086; //Max 1086
    IS_RECT rectAOI;
	rectAOI.s32Width = ImgWidth;
    rectAOI.s32Height = ImgHeight;
    rectAOI.s32X = 0;
    rectAOI.s32Y = 0;
    int nRet2 = is_AOI(hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    is_AOI(hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    cout << "Width: " << rectAOI.s32Width << " Height: " << rectAOI.s32Height << " X: " << rectAOI.s32X << " Y: " << rectAOI.s32Y << endl;

	// ========== Seta e imprime novo FPS ==========
	is_SetFrameRate(hCam, 240, &newFPS); //Ajuste do FPS (newFPS recebe o número de FPS atualizado)
	cout << "FPS: " << newFPS << endl;
    
    // ========== Seta tempo de esposição ==========
    double shutter=3; //Tempo de exposição em ms
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
	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), cameraMatrix, cv::Size(DisplayWidth,DisplayHeight), CV_16SC2, map1, map2 );
	
	while(1){		
		// Registra um frame da câmera
		is_FreezeVideo(hCam, IS_WAIT);
    
		// ========== Carrega imagem para edição com opencv, aplica undistortion e exibe na tela ========== 
		VOID* pMem_b;
		is_GetImageMem(hCam, &pMem_b);
		
		cv::Mat img(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
		
		cv::Mat imgUndistorted;
		
		cv::namedWindow("Distorted", CV_WINDOW_NORMAL);
		cv::imshow("Distorted",img);
		
		cv::Mat tmp = img.clone();
		cv::remap(tmp, img, map1, map2, 1, 0);
		
		cv::namedWindow("Undistorted", CV_WINDOW_NORMAL);
		cv::imshow("Undistorted",img);
		cv::waitKey(1);
		
    }

    // Releases an image memory that was allocated
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);

    // Disables the hCam camera handle and releases the data structures and memory areas taken up by the uEye camera
    is_ExitCamera(hCam);
	
	// Fecha todas as janelas
	cv::destroyAllWindows();

    return 0;
}
