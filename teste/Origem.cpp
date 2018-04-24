#include <iostream>
#include <stdint.h>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

int main(int argv, char** argc) {

	//GRAYSCALE
	Mat file1 = imread("fire_man.jpg", CV_LOAD_IMAGE_COLOR);
	resize(file1, file1, Size(file1.cols / 2, file1.rows / 2));
	Mat file2 = Mat::zeros(file1.rows, file1.cols, CV_8UC1);

	for (int r = 0; r < file2.rows; r++) {
		for (int c = 0; c < file2.cols; c++) {
			//type, 
			file2.at<uchar>(r, c) = 
				file1.at<cv::Vec3b>(r, c)[0] * 0.299 + 
				file1.at<cv::Vec3b>(r, c)[1] * 0.587 + 
				file1.at<cv::Vec3b>(r, c)[2] * 0.114;
		}
	}

	//COLOR
	Mat file3 = imread("fire_man.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	resize(file3, file3, Size(file3.cols / 2, file3.rows / 2));
	Mat file4 = Mat::zeros(file3.rows, file3.cols, CV_8UC1);

	for (int r = 0; r < file4.rows; r++) {
		for (int c = 0; c < file4.cols; c++) {
			//can be done with split and merge
			file4.at<uchar>(r, c) = file3.at<uchar>(r, c) * 0.5;
		}
	} 

	imshow("original", file1);
	imshow("modified", file2);
	imshow("original2", file3);
	imshow("modified2", file4);

	moveWindow("original", 0, 25);
	moveWindow("modified", 0, 25 + file1.rows + 35);
	moveWindow("original2", file1.cols + 15, 25);
	moveWindow("modified2", file2.cols + 15, 25 + file1.rows + 35);

	waitKey();
	system("pause");
	return 0;
}