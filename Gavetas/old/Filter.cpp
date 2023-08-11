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

int main()
{
	cv::Mat img = imread("/home/nvidia/Downloads/Camera/uEye_SimpleSingleGrab/Final.jpg", IMREAD_COLOR);
	cv::Mat imgFinal, GammaCorrection;
	
	img.convertTo(imgFinal, -1, 1.3, 30); //2º algorismo contraste: 2-dobra contraste, 0,5-reduz pela metade
										//3º algorismo brilho, negativo ou positivo
										
	double gamma=0.6;
	Mat lut_matrix(1, 256, CV_8U);
	uchar * ptr = lut_matrix.ptr();
	for(int i=0; i<256; i++)
		ptr[i] = saturate_cast<uchar>(pow(i/255.0, gamma)*255.0);
	
	LUT(img, lut_matrix, GammaCorrection);

	cv::namedWindow("Original", CV_WINDOW_NORMAL);
	cv::imshow("Original",img);
	cv::namedWindow("Final", CV_WINDOW_NORMAL);
	cv::imshow("Final",imgFinal);
	cv::namedWindow("Gamma", CV_WINDOW_NORMAL);
	cv::imshow("Gamma",GammaCorrection);
	cv::waitKey();
	
	// Fecha todas as janelas
	cv::destroyAllWindows();
	
	/*cv::Mat img = imread("Final.jpg", IMREAD_COLOR);
	cv::Mat imgFinal;
	
	cv:: medianBlur(img, imgFinal, 5);

	cv::namedWindow("Original", CV_WINDOW_NORMAL);
	cv::imshow("Original",img);
	cv::namedWindow("Final", CV_WINDOW_NORMAL);
	cv::imshow("Final",imgFinal);
	cv::waitKey();
	
	// Fecha todas as janelas
	cv::destroyAllWindows();*/

    return 0;
}
