#include <iostream>
#include <ueye.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string.h>
#include <sstream>

using namespace std;

int main()
{
	cv::Mat cameraMatrix = (cv::Mat1d(3, 3) << 889.8558214627022, 0, 730.9657076856182,
												0, 889.5253507605797, 512.791593927118,
												0, 0, 1);
	cv::Mat distCoeffs = (cv::Mat1d(1, 5) << -0.3182300709164855, 0.132980716319103, 
											0.002227390313509965, -0.0004478701833075429,
											-0.02919535214796576);
	
	cv::Mat img;
	cv::Mat imgUndistorted;
	img = cv::imread("5.bmp");
	cv::undistort(img, imgUndistorted, cameraMatrix, distCoeffs);
	cv::namedWindow("Distorted", CV_WINDOW_NORMAL);
    cv::imshow("Distorted",img);
	cv::namedWindow("Undistorted", CV_WINDOW_NORMAL);
    cv::imshow("Undistorted",imgUndistorted);
    cv::waitKey(0);

	cv::destroyAllWindows();

    return 0;
}
