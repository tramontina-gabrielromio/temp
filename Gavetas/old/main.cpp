#include <iostream>
#include <ueye.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <sstream>

using namespace std;

HIDS hCam = 0;      // 0 for the next available camera. 1-254 to access by ID
SENSORINFO sInfo;
HWND hWndDisplay;
SENSORSCALERINFO sScaleInfo;

char* pcImageMemory;
int DisplayWidth, DisplayHeight;
double newFPS;

int main()
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   // ========== Inicializa conexão e driver da câmera ==========
    is_InitCamera(&hCam, NULL);
    
    // ========== Define escala de resolução da câmera ==========
    
    //Método SCALER - Não funcionou corretamente
    //int image_down_scale=2;
	//int nSensorScalerInfoSize;
    //int sScale = is_SetSensorScaler(hCam, IS_ENABLE_SENSOR_SCALER, image_down_scale);
    //cout << "SetScale: " << sScale << endl;
    //is_GetSensorScalerInfo(hCam, &sScaleInfo, nSensorScalerInfoSize);
    //cout << "Escala: " << sScaleInfo.dblCurrFactor << endl;
    
    //Método SubSampling
    //int sScale = is_SetSubSampling(hCam, IS_SUBSAMPLING_2X_VERTICAL);
    //cout << "SetSubsamplingVertical: " << sScale << endl;
    //sScale = is_SetSubSampling(hCam, IS_SUBSAMPLING_2X_HORIZONTAL);
    //cout << "SetSubsamplingHorizontal: " << sScale << endl;
    
    //Método Binning - Não funcionou corretamente
    //int sScale = is_SetBinning(hCam, IS_BINNING_4X_VERTICAL);
    //cout << "SetBinningVertical: " << sScale << endl;
    //sScale = is_SetBinning(hCam, IS_BINNING_4X_HORIZONTAL);
    //cout << "SetBinningHorizontal: " << sScale << endl;
      
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
    double shutter=15; //Tempo de exposição em ms
    is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, &shutter, sizeof(shutter));
    is_Exposure(hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, &shutter, sizeof(shutter));
    cout << "Tempo de Exposição: " << shutter << " ms" << endl;
    
    // ========== Carrega informações da câmera e define região para armazenamento da imagem ==========
    is_GetSensorInfo(hCam, &sInfo);
    DisplayWidth = ImgWidth;//sInfo.nMaxWidth;
    DisplayHeight = ImgHeight;//sInfo.nMaxHeight;
    cout << "Resolução: " << DisplayWidth << "x" << DisplayHeight << endl;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Need to find out the memory size of the pixel and the colour mode
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int nMemoryId;
	
    // Assigns a memory for the image and sets it active
    is_AllocImageMem(hCam, DisplayWidth, DisplayHeight, nBitsPerPixel, &pcImageMemory, &nMemoryId);
    is_SetImageMem(hCam, pcImageMemory, nMemoryId);

	int cont=0;
	string imageName;

	while(1){		
		// Acquires a single image from the camera
		is_FreezeVideo(hCam, IS_WAIT);

		/*
		// Parameter definition for saving the image file
		IMAGE_FILE_PARAMS ImageFileParams;
		ImageFileParams.pwchFileName = L"./img.bmp";   /// <-- Insert name and location of the image
		ImageFileParams.pnImageID = NULL;
		ImageFileParams.ppcImageMem = NULL;
		ImageFileParams.nQuality = 0;
		ImageFileParams.nFileType = IS_IMG_BMP;

		// Saves the image file
		if (is_ImageFile(hCam, IS_IMAGE_FILE_CMD_SAVE, (void*)&ImageFileParams, sizeof(ImageFileParams)) == IS_SUCCESS)
		{
			cout << "An Image was saved" << endl;
		}
		else
		{
			cout << "something went wrong" << endl;
		}*/
    
		// ========== Carrega imagem para edição com opencv, exibe na tela e salva ========== 
		VOID* pMem_b;
		is_GetImageMem(hCam, &pMem_b);
		
		cv::Mat mat(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
		cv::namedWindow("Display windows", CV_WINDOW_NORMAL);
		cv::imshow("Display windows", mat);
		imageName = to_string(cont);
		imageName += ".bmp";
		cv::imwrite(imageName, mat);
		cv::waitKey(0);
		cont++;
    }
    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Releases an image memory that was allocated
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);

    // Disables the hCam camera handle and releases the data structures and memory areas taken up by the uEye camera
    is_ExitCamera(hCam);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}
