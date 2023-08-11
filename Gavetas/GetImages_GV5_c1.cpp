#include <iostream>
#include <ueye.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui/highgui_c.h>
#include <unistd.h>
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

Mat MatchingMethod(Mat imgMatching, Mat imgTemplate) //Busca modelo de referência na imagem
{
    //Mat img_display;
    Mat result;
    //imgMatching.copyTo(img_display);
    int result_cols = imgMatching.cols - imgTemplate.cols + 1;
    int result_rows = imgMatching.rows - imgTemplate.rows + 1;
    int resultado = 0;
  
    result.create(result_rows, result_cols, CV_32FC1);
    matchTemplate(imgMatching, imgTemplate, result, TM_CCOEFF_NORMED);
  
    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    //double threshold = 0.92;
  
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    //cout << "Resultado: " << maxVal*100 << "%" << endl;
  
    //if (maxVal >= threshold){
    //    rectangle(img_display, maxLoc, Point(maxLoc.x + imgTemplate.cols, maxLoc.y + imgTemplate.rows), Scalar(255, 255, 255, 0), 2);
    //    resultado = 1;
    //}
    
    //resize(img_display, img_display, Size(), 0.5, 0.5);
    //imshow("Resultado", img_display);

    //imwrite("ToolsGV7/Tool19.png", imgMatching(cv::Rect(maxLoc.x, maxLoc.y, imgTemplate.cols, imgTemplate.rows)));
  
    return imgMatching(cv::Rect(maxLoc.x, maxLoc.y, imgTemplate.cols, imgTemplate.rows));
}

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
    if((is_InitCamera(&hCam, NULL))!=0){
        printf("\nErro ao iniciar a camera...\n");
        *ImagemCam1OK = -1;
        exit(-1);
    }
	
	// ========== Define região de captura da imagem ==========
    int ImgWidth = 640; //Max 1448
    int regiaoScan = 2;
    int ImgHeight = 4; //Max 1086 //100 obs. deve resultar em um número par
    int OffsetX = 280;
    int OffsetY = 1086-620; //obs. o valor é reduzido de 1086 por que a câmera está invertida verticalmente
    
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
    double shutter=0.9; //Tempo de exposição em ms
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
    int contError=0;

	cont = 0;
	ProximoRegistro = 3; //Contagem de pulsos do encoder para próxima captura
	while(*contEncoder>0);
	do{
		if (*contEncoder>=ProximoRegistro){
			//cout << "Cam 1 start - Contador: " << ProximoRegistro << " Encoder 1: " << *contEncoder << endl;
			if((is_FreezeVideo(hCam, IS_WAIT))!=0){
                printf("\nErro ao capturar imagem...\n");
                contError++;
                if (contError>=3){
                    *ImagemCam1OK = -1;
                    exit(-1);
                }
            }
            else{
                contError=0;
            }
			is_GetImageMem(hCam, &pMem_b);
			cv::Mat img1aux(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
            cv::resize(img1aux, img1[cont], cv::Size(), 0.5, 0.5);
			//img1[cont] = img1aux.clone();
			ProximoRegistro = ProximoRegistro+12;
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

    // Salva imagem
	std::string name = "GV5_cam1.png"; //Define endereço das imagens
	imwrite(name, image1F);

	cv::Mat resized;
	resize(image1F, resized, Size(ImgWidth/2, 610/2), 0, 0, INTER_CUBIC);
	name = "GV5_cam1_resized.png";
	imwrite(name, resized);


    // ========== Recorta imagem para treinamento da rede neural ==========
    
    int x,y,width,height,y1,y2,x1,x2;
    std::vector<cv::Point> pts;
    Mat imgMatching; Mat templMatching;
    
	//Ferramenta 1
    y1 = 225;
    y2 = 305;
    x1 = 38;
    x2 = 182;
    y = y1;
    height = y2-y;
    x = x1;
    width = x2-x;
	
	imgMatching = resized(cv::Rect(x, y, width, height)).clone();
    templMatching = imread("RedesGV5/1.png", CV_32FC1);
    cv::Mat tool1 = MatchingMethod(imgMatching, templMatching).clone();
    cv::resize(tool1, tool1, cv::Size(44, 44));
	imwrite("ToolsGV5/Tool1.png", tool1);

	//Ferramenta 3
    y1 = 167;
    y2 = 263;
    x1 = 160;
    x2 = 316;
    y = y1;
    height = y2-y;
    x = x1;
    width = x2-x;
	
	imgMatching = resized(cv::Rect(x, y, width, height)).clone();
    templMatching = imread("RedesGV5/3.png", CV_32FC1);
    cv::Mat tool3 = MatchingMethod(imgMatching, templMatching).clone();
    cv::resize(tool3, tool3, cv::Size(44, 44));
	imwrite("ToolsGV5/Tool3.png", tool3);
	
	//Ferramenta 4
    y1 = 8;
    y2 = 230;
    x1 = 12;
    x2 = 70;
    y = y1;
    height = y2-y;
    x = x1;
    width = x2-x;
	
	imgMatching = resized(cv::Rect(x, y, width, height)).clone();
    templMatching = imread("RedesGV5/4.png", CV_32FC1);
    cv::Mat tool4 = MatchingMethod(imgMatching, templMatching).clone();
    cv::resize(tool4, tool4, cv::Size(44, 44));
	imwrite("ToolsGV5/Tool4.png", tool4);

	//Ferramenta 5
    y1 = 8;
    y2 = 176;
    x1 = 110;
    x2 = 154;
    y = y1;
    height = y2-y;
    x = x1;
    width = x2-x;
	
	imgMatching = resized(cv::Rect(x, y, width, height)).clone();
    templMatching = imread("RedesGV5/5.png", CV_32FC1);
    cv::Mat tool5 = MatchingMethod(imgMatching, templMatching).clone();
    cv::resize(tool5, tool5, cv::Size(44, 44));
	imwrite("ToolsGV5/Tool5.png", tool5);

	//Ferramenta 6
    y1 = 8;
    y2 = 180;
    x1 = 192;
    x2 = 232;
    y = y1;
    height = y2-y;
    x = x1;
    width = x2-x;
	
	imgMatching = resized(cv::Rect(x, y, width, height)).clone();
    templMatching = imread("RedesGV5/6.png", CV_32FC1);
    cv::Mat tool6 = MatchingMethod(imgMatching, templMatching).clone();
    cv::resize(tool6, tool6, cv::Size(44, 44));
	imwrite("ToolsGV5/Tool6.png", tool6);

    *ImagemCam1OK = 1;

	// ========== Fecha todos os dispositivos e janelas ==========
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);
    is_ExitCamera(hCam);
	cv::destroyAllWindows();
	
    return 0;
}
