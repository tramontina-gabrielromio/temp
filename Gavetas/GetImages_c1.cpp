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
                *ImagemCam1OK = -1;
                exit(-1);
            }

            // ========== Define região de captura da imagem ==========
            ImgWidth = 1248; //Max 1448
            regiaoScan = 2;
            ImgHeight = 4; //Max 1086 //100 obs. deve resultar em um número par
            OffsetX = 50;
            OffsetY = 1086-795; //obs. o valor é reduzido de 1086 por que a câmera está invertida verticalmente
            
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
            nColorMode;
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
            std::string name = "GV1_cam1.png"; //Define endereço das imagens
            imwrite(name, image1F);

            cv::Mat resized;
            resize(image1F, resized, Size(ImgWidth/2, 1500/2), 0, 0, INTER_CUBIC);
            name = "GV1_cam1_resized.png";
            imwrite(name, resized);


            // ========== Recorta imagem para treinamento da rede neural ==========

            std::vector<cv::Point> pts;
            Mat imgMatching; Mat templMatching;
            
            //Ferramenta 1
            y = 626;
            height = 750-y;
            x = 102;
            width = 580-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model1.png", CV_32FC1);
            cv::Mat tool1 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool1, tool1, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool1.png", tool1);

            //Ferramenta 2
            y = 554;
            height = 650-y;
            x = 164;
            width = 492-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model2.png", CV_32FC1);
            cv::Mat tool2 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool2, tool2, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool2.png", tool2);

            //Ferramenta 3
            y = 64;
            height = 710-y;
            x = 0;
            width = 110-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model3.png", CV_32FC1);
            cv::Mat tool3 = MatchingMethod(imgMatching, templMatching).clone();
            pts = {cv::Point(102, 130), cv::Point(60, 490), cv::Point(75, 544), cv::Point(102, 544)};
            cv::fillConvexPoly(tool3, pts, cv::Scalar(45, 45, 45));
            cv::resize(tool3, tool3, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool3.png", tool3);

            //Ferramenta 4
            y = 70;
            height = 624-y;
            x = 58;
            width = 176-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model4.png", CV_32FC1);
            cv::Mat tool4 = MatchingMethod(imgMatching, templMatching).clone();
            pts = {cv::Point(32, 0), cv::Point(40, 78), cv::Point(0, 120), cv::Point(0, 0)};
            cv::fillConvexPoly(tool4, pts, cv::Scalar(45, 45, 45));
            pts = {cv::Point(101, 340), cv::Point(80, 529), cv::Point(101, 529)};
            cv::fillConvexPoly(tool4, pts, cv::Scalar(45, 45, 45));
            cv::resize(tool4, tool4, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool4.png", tool4);

            //Ferramenta 5
            y = 70;
            height = 536-y;
            x = 156;
            width = 250-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model5.png", CV_32FC1);
            cv::Mat tool5 = MatchingMethod(imgMatching, templMatching).clone();
            pts = {cv::Point(0, 0), cv::Point(12, 0), cv::Point(12, 76), cv::Point(0, 90)};
            cv::fillConvexPoly(tool5, pts, cv::Scalar(45, 45, 45));
            cv::resize(tool5, tool5, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool5.png", tool5);

            //Ferramenta 6
            y = 72;
            height = 484-y;
            x = 240;
            width = 316-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model6.png", CV_32FC1);
            cv::Mat tool6 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool6, tool6, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool6.png", tool6);

            //Ferramenta 7
            y = 72;
            height = 470-y;
            x = 308;
            width = 376-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model7.png", CV_32FC1);
            cv::Mat tool7 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool7, tool7, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool7.png", tool7);

            //Ferramenta 8
            y = 72;
            height = 416-y;
            x = 378;
            width = 436-x;

            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model8.png", CV_32FC1);
            cv::Mat tool8 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool8, tool8, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool8.png", tool8);

            //Ferramenta 9
            y = 270;
            height = 612-y;
            x = 428;
            width = 624-x;

            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model9.png", CV_32FC1);
            cv::Mat tool9 = MatchingMethod(imgMatching, templMatching).clone();
            pts = {cv::Point(65, 0), cv::Point(85, 98), cv::Point(135, 98), cv::Point(150, 0)};
            cv::fillConvexPoly(tool9, pts, cv::Scalar(45, 45, 45));
            pts = {cv::Point(0, 280), cv::Point(54, 280), cv::Point(60, 308), cv::Point(0, 308)};
            cv::fillConvexPoly(tool9, pts, cv::Scalar(45, 45, 45));
            pts = {cv::Point(186, 250), cv::Point(160, 250), cv::Point(160, 308), cv::Point(186, 308)};
            cv::fillConvexPoly(tool9, pts, cv::Scalar(45, 45, 45));
            cv::resize(tool9, tool9, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool9.png", tool9);

            //Ferramenta 10
            y = 44;
            height = 394-y;
            x = 486;
            width = 602-x;

            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV1/model10.png", CV_32FC1);
            cv::Mat tool10 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool10, tool10, cv::Size(64, 64));
            imwrite("ToolsGV1/Tool10.png", tool10);

            *ImagemCam1OK = 1;
        }
        else if (gvSelecionada==2)
        {
            // ========== Inicializa conexão e driver da câmera ==========
            is_ExitCamera(hCam);
            if((is_InitCamera(&hCam, NULL))!=0){
                printf("\nErro ao iniciar a camera...\n");
                *ImagemCam1OK = -1;
                exit(-1);
            }

            // ========== Define região de captura da imagem ==========
            ImgWidth = 840; //Max 1448
            regiaoScan = 2;
            ImgHeight = 4; //Max 1086 //100 obs. deve resultar em um número par
            OffsetX = 235;
            OffsetY = 1086-735; //obs. o valor é reduzido de 1086 por que a câmera está invertida verticalmente
            
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
            nColorMode;
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
            std::string name = "GV2_cam1.png"; //Define endereço das imagens
            imwrite(name, image1F);

            cv::Mat resized;
            resize(image1F, resized, Size(ImgWidth/2, 1200/2), 0, 0, INTER_CUBIC);
            name = "GV2_cam1_resized.png";
            imwrite(name, resized);


            // ========== Recorta imagem para treinamento da rede neural ==========

            x,y,width,height;
            std::vector<cv::Point> pts;
            Mat imgMatching; Mat templMatching;
            
            //Ferramenta 1
            y = 274;
            height = 574-y;
            x = 10;
            width = 114-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model1.png", CV_32FC1);
            cv::Mat tool1 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool1, tool1, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool1.png", tool1);

            //Ferramenta 2
            y = 320;
            height = 526-y;
            x = 119;
            width = 179-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model2.png", CV_32FC1);
            cv::Mat tool2 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool2, tool2, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool2.png", tool2);

            //Ferramenta 3
            y = 452;
            height = 542-y;
            x = 194;
            width = 232-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model3.png", CV_32FC1);
            cv::Mat tool3 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool3, tool3, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool3.png", tool3);

            //Ferramenta 4
            y = 468;
            height = 542-y;
            x = 254;
            width = 292-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model4.png", CV_32FC1);
            cv::Mat tool4 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool4, tool4, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool4.png", tool4);

            //Ferramenta 5
            y = 402;
            height = 484-y;
            x = 228;
            width = 274-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model5.png", CV_32FC1);
            cv::Mat tool5 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool5, tool5, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool5.png", tool5);

            //Ferramenta 6
            y = 400;
            height = 484-y;
            x = 296;
            width = 342-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model6.png", CV_32FC1);
            cv::Mat tool6 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool6, tool6, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool6.png", tool6);

            //Ferramenta 7
            y = 398;
            height = 484-y;
            x = 354;
            width = 406-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model7.png", CV_32FC1);
            cv::Mat tool7 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool7, tool7, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool7.png", tool7);

            //Ferramenta 8
            y = 17;
            height = 290-y;
            x = 3;
            width = 43-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model8.png", CV_32FC1);
            cv::Mat tool8 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool8, tool8, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool8.png", tool8);

            //Ferramenta 9
            y = 19;
            height = 292-y;
            x = 44;
            width = 84-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model9.png", CV_32FC1);
            cv::Mat tool9 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool9, tool9, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool9.png", tool9);

            //Ferramenta 10
            y = 25;
            height = 290-y;
            x = 86;
            width = 126-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model10.png", CV_32FC1);
            cv::Mat tool10 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool10, tool10, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool10.png", tool10);

            //Ferramenta 11
            y = 30;
            height = 290-y;
            x = 130;
            width = 170-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model11.png", CV_32FC1);
            cv::Mat tool11 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool11, tool11, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool11.png", tool11);

            //Ferramenta 12
            y = 35;
            height = 285-y;
            x = 175;
            width = 217-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model12.png", CV_32FC1);
            cv::Mat tool12 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool12, tool12, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool12.png", tool12);

            /*y = 40;
            height = 278-y;
            x = 180;
            width = 214-x;
            cv::Mat tool12 = resized(cv::Rect(x, y, width, height)).clone(); //(x, y, width, height)
            cv::resize(tool12, tool12, cv::Size(), 0.5, 0.5);
            imwrite("ToolsGV2/Tool12.png", tool12);*/

            //Ferramenta 13
            y = 35;
            height = 283-y;
            x = 223;
            width = 263-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model13.png", CV_32FC1);
            cv::Mat tool13 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool13, tool13, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool13.png", tool13);

            //Ferramenta 14
            y = 30;
            height = 414-y;
            x = 268;
            width = 324-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model14.png", CV_32FC1);
            cv::Mat tool14 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool14, tool14, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool14.png", tool14);

            //Ferramenta 15
            y = 30;
            height = 354-y;
            x = 320;
            width = 372-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model15.png", CV_32FC1);
            cv::Mat tool15 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool15, tool15, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool15.png", tool15);

            //Ferramenta 16
            y = 30;
            height = 318-y;
            x = 374;
            width = 416-x;
            
            imgMatching = resized(cv::Rect(x, y, width, height)).clone();
            templMatching = imread("RedesGV2/model16.png", CV_32FC1);
            cv::Mat tool16 = MatchingMethod(imgMatching, templMatching).clone();
            cv::resize(tool16, tool16, cv::Size(64, 64));
            imwrite("ToolsGV2/Tool16.png", tool16);

            *ImagemCam1OK = 1;
        }
    }

	// ========== Fecha todos os dispositivos e janelas ==========
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);
    is_ExitCamera(hCam);
	cv::destroyAllWindows();
	
    return 0;
}
