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

    int gvSelecionada;
    int nMemoryId;
    int ImgWidth;
    int regiaoScan;
    int ImgHeight;
    int OffsetX;
    int OffsetY;
    double shutter;
    double gamma;
    int nColorMode;
    int nBitsPerPixel;
    int cont;
    int contError;
    int x,y,width,height;

    while(1){
        cout << "\nSelecione uma gaveta: ";
        cin >> gvSelecionada;

        if (gvSelecionada==1){
            // ========== Inicializa conexão e driver da câmera ==========
            is_ExitCamera(hCam);
            if((is_InitCamera(&hCam, NULL))!=0){
                printf("\nErro ao iniciar a camera...\n");
                *ImagemCam2OK = -1;
                exit(-1);
            }
	
            // ========== Define região de captura da imagem ==========
            ImgWidth = 1184; //Max 1448
            regiaoScan = 2;
            ImgHeight = 4; //Max 1086 //100 obs. deve resultar em um número par
            OffsetX = 125;
            OffsetY = 845;
            
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
            shutter=0.5; //Tempo de exposição em ms
            is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, &shutter, sizeof(shutter));
            is_Exposure(hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, &shutter, sizeof(shutter));
            cout << "Tempo de Exposição: " << shutter << " ms" << endl;
            
            // ========== Seta e imprime novo FPS ==========
            //Obs. Insere o maior valor possível de acordo com o tempo de exposição
            is_SetFrameRate(hCam, 1000/shutter, &newFPS); //Ajuste do FPS (newFPS recebe o número de FPS atualizado)
            cout << "FPS: " << newFPS << endl;
            
            // ========== Configura gamma ==========
            gamma=0.5; //1 é a imagem normal, quanto menor mais claro
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
            nBitsPerPixel = 24;

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
            
            // Assigns a memory for the image and sets it active
            is_AllocImageMem(hCam, DisplayWidth, DisplayHeight, nBitsPerPixel, &pcImageMemory, &nMemoryId);
            is_SetImageMem(hCam, pcImageMemory, nMemoryId);
            
            cont=0;
            VOID* pMem_b;
            cv::Mat img1[1000];
            cv::Mat imgUndistorted;
            cv::Mat tmp1a(1086, 1448, CV_8UC3), tmp1b(1086, 1448, CV_8UC3);
            cv::Mat image1F, image2F;
            cv::Mat img1aux;
            contError=0;

            cont = 0;
            ProximoRegistro = 1; //Contagem de pulsos do encoder para próxima captura
            while(*contEncoder>0);
            do{
                if (*contEncoder>=ProximoRegistro){
                    //cout << "Cam 1 start - Contador: " << ProximoRegistro << " Encoder 1: " << *contEncoder << endl;
                    if((is_FreezeVideo(hCam, IS_WAIT))!=0){
                        printf("\nErro ao capturar imagem...\n");
                        contError++;
                        if (contError>=3){
                            *ImagemCam2OK = -1;
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
                    ProximoRegistro = ProximoRegistro+24;
                    cont++;
                    //cout << "Cam 1 end  - Encoder: " << *contEncoder << " Total: " << *contEncoder-ProximoRegistro+32 << endl;
                    //cout << "Contador: " << *contEncoder << "\nCont: " << cont << endl;
                }
            }while(*contEncoder!=0 || cont==0);
            
            //Concatena região de interesse das imagens obtidas anteriormente em uma única imagem
            image1F = img1[1](Rect(0, 0, ImgWidth/2, regiaoScan)).clone();
            for (int cont2=2; cont2<cont-1; cont2++){
                vconcat(img1[cont2](Rect(0, 0, ImgWidth/2, regiaoScan)), image1F, image1F);
            }
                    
            // Aplica correção de gamma
            LUT(image1F, lut_matrix, image1F);
            
            // Salva imagem
            std::string name = "GV1_cam2.png"; //Define endereço das imagens
            imwrite(name, image1F);

            cv::Mat resized;
            resize(image1F, resized, Size(ImgWidth/2, 1500/2), 0, 0, INTER_CUBIC);
            name = "GV1_cam2_resized.png";
            imwrite(name, resized);


            // ========== Recorta imagem para treinamento da rede neural ==========

            std::vector<cv::Point> pts;
            Mat imgMatching; Mat templMatching;

            //Ferramenta 11
            y = 620;
            height = 750-y;
            x = 0;
            width = 564-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model11.png", CV_32FC1);
            cv::Mat tool11 = MatchingMethod(imgMatching, templMatching).clone();
            pts = {cv::Point(360, 0), cv::Point(360, 22), cv::Point(500, 22), cv::Point(500, 0)};
            cv::fillConvexPoly(tool11, pts, cv::Scalar(45, 45, 45));
            cv::resize(tool11, tool11, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool11.png", tool11);

            //Ferramenta 12
            y = 538;
            height = 616-y;
            x = 0;
            width = 44-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model12.png", CV_32FC1);
            cv::Mat tool12 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool12, tool12, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool12.png", tool12);

            //Ferramenta 13
            y = 550;
            height = 626-y;
            x = 50;
            width = 104-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model13.png", CV_32FC1);
            cv::Mat tool13 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool13, tool13, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool13.png", tool13);

            //Ferramenta 14
            y = 562;
            height = 640-y;
            x = 134;
            width = 196-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model14.png", CV_32FC1);
            cv::Mat tool14 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool14, tool14, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool14.png", tool14);

            //Ferramenta 15
            y = 570;
            height = 644-y;
            x = 220;
            width = 272-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model15.png", CV_32FC1);
            cv::Mat tool15 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool15, tool15, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool15.png", tool15);

            //Ferramenta 16
            y = 568;
            height = 642-y;
            x = 302;
            width = 346-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model16.png", CV_32FC1);
            cv::Mat tool16 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool16, tool16, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool16.png", tool16);

            //Ferramenta 17
            y = 488;
            height = 576-y;
            x = 100;
            width = 326-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model17.png", CV_32FC1);
            cv::Mat tool17 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool17, tool17, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool17.png", tool17);

            //Ferramenta 18
            y = 520;
            height = 668-y;
            x = 354;
            width = 438-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model18.png", CV_32FC1);
            cv::Mat tool18 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool18, tool18, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool18.png", tool18);

            //Ferramenta 19
            y = 520;
            height = 664-y;
            x = 440;
            width = 512-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model19.png", CV_32FC1);
            cv::Mat tool19 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool19, tool19, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool19.png", tool19);

            //Ferramenta 20
            y = 550;
            height = 636-y;
            x = 528;
            width = 592-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model20.png", CV_32FC1);
            cv::Mat tool20 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool20, tool20, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool20.png", tool20);

            //Ferramenta 21
            y = 42;
            height = 516-y;
            x = 4;
            width = 134-x;

            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model21.png", CV_32FC1);
            cv::Mat tool21 = MatchingMethod(imgMatching, templMatching).clone();
            pts = {cv::Point(0, 234), cv::Point(16, 234), cv::Point(16, 380), cv::Point(0, 434)};
            cv::fillConvexPoly(tool21, pts, cv::Scalar(45, 45, 45));
            cv::resize(tool21, tool21, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool21.png", tool21);

            //Ferramenta 22
            y = 50;
            height = 356-y;
            x = 124;
            width = 204-x;

            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model22.png", CV_32FC1);
            cv::Mat tool22 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool22, tool22, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool22.png", tool22);

            //Ferramenta 23
            y = 54;
            height = 494-y;
            x = 204;
            width = 302-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model23.png", CV_32FC1);
            cv::Mat tool23 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool23, tool23, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool23.png", tool23);

            //Ferramenta 24
            y = 50;
            height = 496-y;
            x = 320;
            width = 416-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model24.png", CV_32FC1);
            cv::Mat tool24 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool24, tool24, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool24.png", tool24);

            //Ferramenta 25
            y = 46;
            height = 480-y;
            x = 418;
            width = 506-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model25.png", CV_32FC1);
            cv::Mat tool25 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool25, tool25, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool25.png", tool25);

            //Ferramenta 26
            y = 42;
            height = 566-y;
            x = 506;
            width = 588-x;

            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model26.png", CV_32FC1);
            cv::Mat tool26 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool26, tool26, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool26.png", tool26);

            *ImagemCam2OK = 1;
        }
        else if (gvSelecionada==2)
        {
            // ========== Inicializa conexão e driver da câmera ==========
            is_ExitCamera(hCam);
            if((is_InitCamera(&hCam, NULL))!=0){
                printf("\nErro ao iniciar a camera...\n");
                *ImagemCam2OK = -1;
                exit(-1);
            }

            // ========== Define região de captura da imagem ==========
            ImgWidth = 960; //Max 1448
            regiaoScan = 2;
            ImgHeight = 4; //Max 1086 //100 obs. deve resultar em um número par
            OffsetX = 210;
            OffsetY = 745;
            
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
            shutter=0.6; //Tempo de exposição em ms
            is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, &shutter, sizeof(shutter));
            is_Exposure(hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, &shutter, sizeof(shutter));
            cout << "Tempo de Exposição: " << shutter << " ms" << endl;
            
            // ========== Seta e imprime novo FPS ==========
            //Obs. Insere o maior valor possível de acordo com o tempo de exposição
            is_SetFrameRate(hCam, 1000/shutter, &newFPS); //Ajuste do FPS (newFPS recebe o número de FPS atualizado)
            cout << "FPS: " << newFPS << endl;
            
            // ========== Configura gamma ==========
            gamma=0.5; //1 é a imagem normal, quanto menor mais claro
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
            nBitsPerPixel = 24;

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
            
            // Assigns a memory for the image and sets it active
            is_AllocImageMem(hCam, DisplayWidth, DisplayHeight, nBitsPerPixel, &pcImageMemory, &nMemoryId);
            is_SetImageMem(hCam, pcImageMemory, nMemoryId);
            
            cont=0;
            VOID* pMem_b;
            cv::Mat img1[1000];
            cv::Mat imgUndistorted;
            cv::Mat tmp1a(1086, 1448, CV_8UC3), tmp1b(1086, 1448, CV_8UC3);
            cv::Mat image1F, image2F;
            cv::Mat img1aux;
            contError=0;

            cont = 0;
            ProximoRegistro = 1; //Contagem de pulsos do encoder para próxima captura
            while(*contEncoder>0);
            do{
                if (*contEncoder>=ProximoRegistro){
                    //cout << "Cam 1 start - Contador: " << ProximoRegistro << " Encoder 1: " << *contEncoder << endl;
                    if((is_FreezeVideo(hCam, IS_WAIT))!=0){
                        printf("\nErro ao capturar imagem...\n");
                        contError++;
                        if (contError>=3){
                            *ImagemCam2OK = -1;
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
                    ProximoRegistro = ProximoRegistro+24;
                    cont++;
                    //cout << "Cam 1 end  - Encoder: " << *contEncoder << " Total: " << *contEncoder-ProximoRegistro+32 << endl;
                    //cout << "Contador: " << *contEncoder << "\nCont: " << cont << endl;
                }
            }while(*contEncoder!=0 || cont==0);
            
            //Concatena região de interesse das imagens obtidas anteriormente em uma única imagem
            image1F = img1[1](Rect(0, 0, ImgWidth/2, regiaoScan)).clone();
            for (int cont2=2; cont2<cont-1; cont2++){
                vconcat(img1[cont2](Rect(0, 0, ImgWidth/2, regiaoScan)), image1F, image1F);
            }
                    
            // Aplica correção de gamma
            LUT(image1F, lut_matrix, image1F);
            
            // Salva imagem
            std::string name = "GV2_cam2.png"; //Define endereço das imagens
            imwrite(name, image1F);

            cv::Mat resized;
            resize(image1F, resized, Size(ImgWidth/2, 1200/2), 0, 0, INTER_CUBIC);
            name = "GV2_cam2_resized.png";
            imwrite(name, resized);


            // ========== Recorta imagem para treinamento da rede neural ==========

            std::vector<cv::Point> pts;
            Mat imgMatching; Mat templMatching;

            //Ferramenta 17
            y = 490;
            height = 600-y;
            x = 0;
            width = 334-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model17.png", CV_32FC1);
            cv::Mat tool17 = MatchingMethod(imgMatching, templMatching).clone();
            pts = {cv::Point(180, 0), cv::Point(180, 16), cv::Point(284, 16), cv::Point(284, 0)};
            cv::fillConvexPoly(tool17, pts, cv::Scalar(45, 45, 45));
            cv::resize(tool17, tool17, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool17.png", tool17);

            //Ferramenta 18
            y = 422;
            height = 498-y;
            x = 0;
            width = 48-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model18.png", CV_32FC1);
            cv::Mat tool18 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool18, tool18, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool18.png", tool18);

            //Ferramenta 19
            y = 424;
            height = 500-y;
            x = 50;
            width = 102-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model19.png", CV_32FC1);
            cv::Mat tool19 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool19, tool19, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool19.png", tool19);

            //Ferramenta 20
            y = 430;
            height = 504-y;
            x = 118;
            width = 168-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model20.png", CV_32FC1);
            cv::Mat tool20 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool20, tool20, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool20.png", tool20);

            //Ferramenta 21
            y = 408;
            height = 528-y;
            x = 176;
            width = 234-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model21.png", CV_32FC1);
            cv::Mat tool21 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool21, tool21, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool21.png", tool21);

            //Ferramenta 22
            y = 408;
            height = 528-y;
            x = 238;
            width = 296-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model22.png", CV_32FC1);
            cv::Mat tool22 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool22, tool22, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool22.png", tool22);

            //Ferramenta 23
            y = 438;
            height = 502-y;
            x = 308;
            width = 342-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model23.png", CV_32FC1);
            cv::Mat tool23 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool23, tool23, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool23.png", tool23);

            //Ferramenta 24
            y = 64;
            height = 290-y;
            x = 26;
            width = 60-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model24.png", CV_32FC1);
            cv::Mat tool24 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool24, tool24, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool24.png", tool24);

            //Ferramenta 25
            y = 64;
            height = 436-y;
            x = 68;
            width = 180-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model25.png", CV_32FC1);
            cv::Mat tool25 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool25, tool25, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool25.png", tool25);

            //Ferramenta 26
            y = 80;
            height = 350-y;
            x = 196;
            width = 256-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model26.png", CV_32FC1);
            cv::Mat tool26 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool26, tool26, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool26.png", tool26);

            //Ferramenta 27
            y = 80;
            height = 434-y;
            x = 270;
            width = 348-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model27.png", CV_32FC1);
            cv::Mat tool27 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool27, tool27, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool27.png", tool27);

            //Ferramenta 28
            y = 64;
            height = 568-y;
            x = 336;
            width = 480-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model28.png", CV_32FC1);
            cv::Mat tool28 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool28, tool28, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool28.png", tool28);

            *ImagemCam2OK = 1;
        }
    }
    
	// ========== Fecha todos os dispositivos e janelas ==========
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);
    is_ExitCamera(hCam);
	cv::destroyAllWindows();
	
    return 0;
}
