#include <iostream>
#include <stdint.h>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argv, char** argc) {

	Mat input_image;
	Mat output_image;

	VideoCapture vid(0);

	input_image = vid.read(input_image);
	if (!vid.isOpened())
		return -1;

	//create an empty input_image (size X, size Y, Type) - number 1 = 1 channels 
	output_image = Mat::zeros(input_image.rows, input_image.cols, CV_8UC1);
	
	//	cvtColor(input_image, output_image, CV_RGB2GRAY);

	//pixel
	while (vid.read(input_image)) {
		for (int r = 0; r < input_image.rows; r++) {
			for (int c = 0; c < input_image.cols; c++) {
					output_image.at<uchar>(r, c) = input_image.at<cv::Vec3b>(r, c)[0] * 0.33 +
													input_image.at<cv::Vec3b>(r, c)[1] * 0.33 +
													input_image.at<cv::Vec3b>(r, c)[2] * 0.34;

					if (output_image.at<uchar>(r, c) <= 124)
						output_image.at<uchar>(r, c) = 0;
					else
						output_image.at<uchar>(r, c) = 255;
			}
		}
		imshow("webcam1", output_image);

		if (waitKey(1) == 27)
			break;
	}
	return 0;
}