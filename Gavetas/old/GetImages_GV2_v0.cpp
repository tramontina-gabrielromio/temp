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
	
	// ========== Matrizes de calibração do sistema ==========
	/*cv::Mat cameraMatrix = (cv::Mat1d(3, 3) << 	889.8558214627022, 		0, 						730.9657076856182,
												0, 						889.5253507605797, 		512.791593927118,
												0, 						0, 						1);
												
	cv::Mat distCoeffs = (cv::Mat1d(1, 5) << 	-0.3182300709164855, 	0.132980716319103, 		0.002227390313509965, 
												-0.0004478701833075429,	-0.02919535214796576);*/
	cv::Mat cameraMatrix = (cv::Mat1d(3, 3) << 	892.1691931981718, 		0, 						724.1965286470058,
												0, 						892.2270082418462, 		586.1900181919949,
												0, 						0, 						1);
												
	cv::Mat distCoeffs = (cv::Mat1d(1, 5) << 	-0.3319552721123589, 	0.1552019675899306, 		-0.001548766723012425, 
												-9.371354036299631e-05,	-0.04069783306301979);
	
	// ========== Inicializa conexão e driver da câmera ==========
    is_InitCamera(&hCam, NULL);
	
	// ========== Define região de captura da imagem ==========
    int ImgWidth = 1000; //Max 1448
    int ImgHeight = 80; //Max 1086 //100
    int OffsetX = 200;
    int OffsetY = 700;
    int regiaoScan=13;
    
    IS_RECT rectAOI;
	rectAOI.s32Width = ImgWidth;
    rectAOI.s32Height = ImgHeight;
    rectAOI.s32X = OffsetX;
    rectAOI.s32Y = OffsetY;
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
	map1 = map3(cv::Rect(0, OffsetY, 1448, 100)).clone();//150
	map2 = map4(cv::Rect(0, OffsetY, 1448, 100)).clone();//150
	
	int cont=0;
	VOID* pMem_b;			
	cv::Mat img(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
	cv::Mat imgUndistorted;
	cv::Mat tmp(1086, 1448, CV_8UC3), tmp2(1086, 1448, CV_8UC3);
	cv::Mat image[87];
	cv::Mat imageF;
	
	cv::namedWindow("Image", CV_WINDOW_NORMAL);
	cv::waitKey(1);
	
	while(1){
		cont=0;
		ProximoRegistro = 1;
		while(*contEncoder>0);
		cv::imshow("Image", 255);
		do{		
			if (*contEncoder>=ProximoRegistro){
				
				is_FreezeVideo(hCam, IS_WAIT);
				is_GetImageMem(hCam, &pMem_b);
				cv::Mat img(DisplayHeight, DisplayWidth, CV_8UC3, pMem_b);
				img.copyTo(tmp(cv::Rect(OffsetX, OffsetY, img.cols, img.rows)));
				
				cv::remap(tmp, tmp2, map1, map2, 1, 0);
				
				cont++;
				image[cont] = tmp2(cv::Rect(OffsetX, 24, ImgWidth, regiaoScan)).clone();
				cv::waitKey(5);
		
				if (ProximoRegistro == 1)
					ProximoRegistro = 32;//120
				else
					ProximoRegistro = ProximoRegistro+32;//120
				cout << "Contador: " << *contEncoder << "\nCont: " << cont << endl;
			}
		}while(cont<85);//52
		
		imageF = image[1].clone();
		for (int cont2=2; cont2<=cont; cont2++)
			vconcat(image[cont2], imageF, imageF);
		
		// Aplica correção de gamma
		LUT(imageF, lut_matrix, imageF);

		// Exibe e sava imagem
		imwrite("0.png", imageF);
		cv::namedWindow("Image", CV_WINDOW_NORMAL);
		cv::imshow("Image",imageF);
		cv::waitKey(1);
		
		/*
		// ========== Recorta imagem para treinamento da rede neural ==========
		
		//Ferramenta 1
		cv::Mat tool1(90, 625, CV_8UC3);
		cv::Mat toolTemp1 = imageF(cv::Rect(90, 210, 180, 1250)).clone(); //(x, y, width, height)
		cv::resize(toolTemp1, tool1, cv::Size(), 0.5, 0.5);
		imwrite("Tool1.png", tool1);
		//Ponteira 1
		cv::Mat pont1 = imageF(cv::Rect(110, 1230, 140, 230)).clone(); //(x, y, width, height)
		imwrite("Pont1.png", pont1);
		
		//Ferramenta 2
		cv::Mat tool2(90, 625, CV_8UC3);
		cv::Mat toolTemp2 = imageF(cv::Rect(360, 210, 180, 1250)).clone();
		cv::resize(toolTemp2, tool2, cv::Size(), 0.5, 0.5);
		imwrite("Tool2.png", tool2);
		//Ponteira 2
		cv::Mat pont2 = imageF(cv::Rect(380, 1230, 140, 230)).clone(); //(x, y, width, height)
		imwrite("Pont2.png", pont2);
		
		//Ferramenta 3
		cv::Mat tool3(90, 625, CV_8UC3);
		cv::Mat toolTemp3 = imageF(cv::Rect(620, 210, 180, 1250)).clone();
		cv::resize(toolTemp3, tool3, cv::Size(), 0.5, 0.5);
		imwrite("Tool3.png", tool3);
		//Ponteira 3
		cv::Mat pont3(140, 230, CV_8UC3);
		pont3 = imageF(cv::Rect(640, 1230, 140, 230)).clone(); //(x, y, width, height)
		imwrite("Pont3.png", pont3);
		
		//Ferramenta 4
		cv::Mat tool4(90, 625, CV_8UC3);
		cv::Mat toolTemp4 = imageF(cv::Rect(885, 210, 180, 1250)).clone();
		cv::resize(toolTemp4, tool4, cv::Size(), 0.5, 0.5);
		imwrite("Tool4.png", tool4);
		//Ponteira 4
		cv::Mat pont4(140, 230, CV_8UC3);
		pont4 = imageF(cv::Rect(895, 1230, 140, 230)).clone(); //(x, y, width, height) // 640, 1230, 140, 230 // 895, 1230, 140, 230)
		imwrite("Pont4.png", pont4);
		
		//Ferramenta 5
		cv::Mat tool5(90, 625, CV_8UC3);
		cv::Mat toolTemp5 = imageF(cv::Rect(1140, 186, 200, 1274)).clone();
		cv::resize(toolTemp5, tool5, cv::Size(), 0.5, 0.5);
		imwrite("Tool5.png", tool5);
		//Ponteira 5
		cv::Mat pont5 = imageF(cv::Rect(1140, 1230, 160, 230)).clone(); //(x, y, width, height)
		imwrite("Pont5.png", pont5);
		
		//Processa imagens registradas na rede neural
		system("clear");
		
		Mat imageFV2 = imageF.clone();
		
		if ((RedeTool1() + RedePont1())==2){
			cv::rectangle(imageFV2, cv::Point(2*90, 2*210), cv::Point(2*270, 2*1460), cv::Scalar(0, 255, 0, 0), 4, LINE_8, 1);
			cv::putText(imageFV2, "Correto", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		}
		else{
			cv::rectangle(imageFV2, cv::Point(2*90, 2*210), cv::Point(2*270, 2*1460), cv::Scalar(0, 0, 255, 0), 4, LINE_8, 1);
			cv::putText(imageFV2, "Errado", cv::Point(90, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		}
				
		if ((RedeTool2() + RedePont2())==2){
			cv::rectangle(imageFV2, cv::Point(2*360, 2*210), cv::Point(2*540, 2*1460), cv::Scalar(0, 255, 0, 0), 4, LINE_8, 1);
			cv::putText(imageFV2, "Correto", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		}
		else{
			cv::rectangle(imageFV2, cv::Point(2*360, 2*210), cv::Point(2*540, 2*1460), cv::Scalar(0, 0, 255, 0), 4, LINE_8, 1);
			cv::putText(imageFV2, "Errado", cv::Point(360, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		}
		
		if ((RedeTool3() + RedePont3())==2){
			cv::rectangle(imageFV2, cv::Point(2*620, 2*210), cv::Point(2*800, 2*1460), cv::Scalar(0, 255, 0, 0), 4, LINE_8, 1);
			cv::putText(imageFV2, "Correto", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		}
		else{
			cv::rectangle(imageFV2, cv::Point(2*620, 2*210), cv::Point(2*800, 2*1460), cv::Scalar(0, 0, 255, 0), 4, LINE_8, 1);
			cv::putText(imageFV2, "Errado", cv::Point(620, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		}
		
		if ((RedeTool4() + RedePont4())==2){
			cv::rectangle(imageFV2, cv::Point(2*885, 2*210), cv::Point(2*1065, 2*1460), cv::Scalar(0, 255, 0, 0), 4, LINE_8, 1);
			cv::putText(imageFV2, "Correto", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		}
		else{
			cv::rectangle(imageFV2, cv::Point(2*885, 2*210), cv::Point(2*1065, 2*1460), cv::Scalar(0, 0, 255, 0), 4, LINE_8, 1);
			cv::putText(imageFV2, "Errado", cv::Point(885, 180), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		}
		
		if ((RedeTool5() + RedePont5())==2){
			cv::rectangle(imageFV2, cv::Point(2*1140, 2*186), cv::Point(2*1340, 2*1460), cv::Scalar(0, 255, 0, 0), 4, LINE_8, 1);
			cv::putText(imageFV2, "Correto", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0, 0), 4, LINE_8, false);
		}
		else{
			cv::rectangle(imageFV2, cv::Point(2*1140, 2*186), cv::Point(2*1340, 2*1460), cv::Scalar(0, 0, 255, 0), 4, LINE_8, 1);
			cv::putText(imageFV2, "Errado", cv::Point(1140, 156), FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255, 0), 4, LINE_8, false);
		}
		
		imwrite("Final.jpg", imageFV2);
		cv::imshow("Image",imageFV2);
		cv::waitKey(1);
	*/
	}
	
	
	// ========== Fecha todos os dispositivos e janelas ==========
    is_FreeImageMem(hCam, pcImageMemory, nMemoryId);
    is_ExitCamera(hCam);
	cv::destroyAllWindows();
	
    return 0;
}
