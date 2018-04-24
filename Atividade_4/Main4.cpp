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
	uchar *p = output_image.data;

	while (vid.read(input_image)) {
		cvtColor(input_image, output_image, CV_RGB2GRAY);
		for (int r = 0; r < input_image.rows*input_image.cols; r++) {
				if (p[r] <= 128)
					p[r] = 0;
				else
					p[r] = 255;
		}
		imshow("webcam1", output_image);

		if (waitKey(1) == 27)
			break;
	}

	system("pause");
	return 0;
}