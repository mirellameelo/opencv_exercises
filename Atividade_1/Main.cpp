#include <iostream>
#include <stdint.h>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;
using namespace std;

const int fps = 20;

int main(int argv, char** argc) {
	
	Mat frame;
	Mat frame1, frame2, frame3;

	VideoCapture vid(0);

	frame = vid.read(frame);
	if (!vid.isOpened())
		return -1;

	//create an empty frame (size X, size Y, Type) - number 3 = 3 channels 
	frame1 = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
	frame2 = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
	frame3 = Mat::zeros(frame.rows, frame.cols, CV_8UC3);

	while (vid.read(frame)) {
		for (int r = 0; r < frame.rows; r++) {
			for (int c = 0; c < frame.cols; c++) {
				//can be done with split and merge
				frame1.at<Vec3b>(r, c)[0] = frame.at<Vec3b>(r, c)[0];
				frame2.at<Vec3b>(r, c)[1] = frame.at<Vec3b>(r, c)[1];
				frame3.at<Vec3b>(r, c)[2] = frame.at<Vec3b>(r, c)[2];
			}
		}
		imshow("webcam1", frame1);
		imshow("webcam2", frame2);
		imshow("webcam3", frame3);

		if (waitKey(1) == 27)
			break;
	}
	return 0;
}