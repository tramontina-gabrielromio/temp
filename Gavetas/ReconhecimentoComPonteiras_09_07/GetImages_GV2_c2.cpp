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
    int OffsetX = 260;
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
	ProximoRegistro = 1; //Contagem de pulsos do encoder para próxima captura
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
	std::string name = "GV2_cam2.png"; //Define endereço das imagens
	imwrite(name, image1F);

	cv::Mat resized;
	resize(image1F, resized, Size(ImgWidth, 1100), 0, 0, INTER_CUBIC);
	name = "GV2_cam2_resized.png";
	imwrite(name, resized);


	// ========== Recorta imagem para treinamento da rede neural ==========

    int x,y,width,height;
	
	//Ferramenta 1
    y = 820;
    height = 1100-y;
    x = 85;
    width = 205-x;
	cv::Mat tool1 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool1, tool1, cv::Size(), 0.5, 0.5);
	imwrite("Tool1.png", tool1);

	//Ponteira 1
    y = 820;
    height = 940-y;
    x = 125;
    width = 165-x;
	cv::Mat pont1 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont1.png", pont1);

	//Ferramenta 2
    y = 830;
    height = 1100-y;
    x = 230;
    width = 350-x;
	cv::Mat tool2 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool2, tool2, cv::Size(), 0.5, 0.5);
	imwrite("Tool2.png", tool2);

	//Ponteira 2
    y = 830;
    height = 950-y;
    x = 270;
    width = 310-x;
	cv::Mat pont2 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont2.png", pont2);

	//Ferramenta 3
    y = 830;
    height = 1100-y;
    x = 385;
    width = 515-x;
	cv::Mat tool3 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool3, tool3, cv::Size(), 0.5, 0.5);
	imwrite("Tool3.png", tool3);

	//Ponteira 3
    y = 830;
    height = 940-y;
    x = 425;
    width = 475-x;
	cv::Mat pont3 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont3.png", pont3);

	//Ferramenta 4
    y = 200;
    height = 850-y;
    x = 90;
    width = 200-x;
	cv::Mat tool4 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool4, tool4, cv::Size(), 0.5, 0.5);
	imwrite("Tool4.png", tool4);

	//Ponteira 4
    y = 700;
    height = 840-y;
    x = 135;
    width = 165-x;
	cv::Mat pont4 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont4.png", pont4);

	//Ferramenta 5
    y = 185;
    height = 855-y;
    x = 235;
    width = 355-x;
	cv::Mat tool5 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool5, tool5, cv::Size(), 0.5, 0.5);
	imwrite("Tool5.png", tool5);

	//Ponteira 5
    y = 685;
    height = 835-y;
    x = 275;
    width = 315-x;
	cv::Mat pont5 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont5.png", pont5);

	//Ferramenta 6
    y = 185;
    height = 855-y;
    x = 390;
    width = 510-x;
	cv::Mat tool6 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool6, tool6, cv::Size(), 0.5, 0.5);
	imwrite("Tool6.png", tool6);

	//Ponteira 6
    y = 705;
    height = 845-y;
    x = 425;
    width = 465-x;
	cv::Mat pont6 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont6.png", pont6);

	//Ferramenta 7
    y = 284;
    height = 1000-y;
    x = 600;
    width = 720-x;
	cv::Mat tool7 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool7, tool7, cv::Size(), 0.5, 0.5);
	imwrite("Tool7.png", tool7);

	//Ponteira 7
    y = 880;
    height = 1000-y;
    x = 632;
    width = 672-x;
	cv::Mat pont7 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont7.png", pont7);

	//Ferramenta 8
    y = 320;
    height = 1000-y;
    x = 740;
    width = 860-x;
	cv::Mat tool8 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    cv::resize(tool8, tool8, cv::Size(), 0.5, 0.5);
	imwrite("Tool8.png", tool8);

	//Ponteira 8
    y = 880;
    height = 1000-y;
    x = 768;
    width = 808-x;
	cv::Mat pont8 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
    imwrite("Pont8.png", pont8);

    
	// ========== Fecha todos os dispositivos e janelas ==========
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);
    is_ExitCamera(hCam);
	cv::destroyAllWindows();
	
    return 0;
}
