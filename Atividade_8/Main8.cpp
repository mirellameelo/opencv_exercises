#include <iostream>
#include <stdint.h>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argv, char** argc) {

	Mat image = imread("fire_man.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat edges = Mat::zeros(image.rows, image.cols, CV_8UC1);
	int max = 0;
	int min = 0;

	namedWindow("Range", CV_WINDOW_AUTOSIZE);
	createTrackbar("max", "Range", &max, 255);
	createTrackbar("min", "Range", &min, 100);

	while (true) {
		Canny(image, edges, min, max, 3);

		imshow("edges", edges);

		if (waitKey(1) == 27)
			break;
	}

	return 0;

}