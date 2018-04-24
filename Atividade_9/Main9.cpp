#include <iostream>
#include <stdint.h>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;
using namespace std;


int main(int argv, char** argc) {

	Mat atual_frame;
	Mat past_frame;
	Mat diference;

	VideoCapture video(0);

	atual_frame = video.read(atual_frame);
	if (!video.isOpened())
		return -1;

	past_frame = Mat::zeros(atual_frame.rows, atual_frame.cols, CV_8UC1);

	while (video.read(atual_frame)) {
		//restart matrix 
		cvtColor(atual_frame, atual_frame, CV_RGB2GRAY);
		diference = Mat::zeros(atual_frame.rows, atual_frame.cols, CV_8UC1);

		diference = atual_frame - past_frame;
		imshow("webcam", diference);

		past_frame = atual_frame;

		if (waitKey(1) == 27)
			break;
	}
}