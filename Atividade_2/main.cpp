#include <iostream>
#include <stdint.h>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>

//COLOCAR AQUI ATIVIDADE 2

using namespace cv;
using namespace std;


int main(int argv, char** argc) {

	Mat frame;
	Mat frame1;

	VideoCapture vid(0);

	frame = vid.read(frame);

	if (!vid.isOpened())
		return -1;

	//create an empty frame (size X, size Y, Type) - number 1 = 1 channel
	frame1 = Mat::zeros(frame.rows, frame.cols, CV_8UC1);

	while (vid.read(frame)) {
		for (int r = 0; r < frame.rows; r++) {
			for (int c = 0; c < frame.cols; c++) {
				//can be done with split and merge
				frame1.at<uchar>(r, c) = frame.at<Vec3b>(r, c)[0] * 0.299 +
					frame.at<Vec3b>(r, c)[1] * 0.587 +
					frame.at<Vec3b>(r, c)[2] * 0.114;

			}
		}
		imshow("webcam1", frame1);

		if (waitKey(1) == 27)
			break;
	}
	return 0;
}