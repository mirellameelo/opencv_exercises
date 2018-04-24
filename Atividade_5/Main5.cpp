#include <iostream>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

int hmax,hmin,smax,smin,vmax,vmin;
Mat slide_bar;

int main(int argv, char** argc) {

	Mat input_image;
	Mat output_image_gray;
	Mat output_image_color;

	VideoCapture vid(0);

	namedWindow("Range");

	createTrackbar("hmax", "Range", &hmax, 255);
	createTrackbar("hmin", "Range", &hmin, 255);
	createTrackbar("smax", "Range", &smax, 255);
	createTrackbar("smin", "Range", &smin, 255);
	createTrackbar("vmax", "Range", &vmax, 255);
	createTrackbar("vmin", "Range", &vmin, 255);

	input_image = vid.read(input_image);
	if (!vid.isOpened())
		return -1;

	//create an empty input_image (size X, size Y, Type) - number 1 = 1 channels 

	while (vid.read(input_image)) {
		//restart matrix 
		output_image_gray = Mat::zeros(input_image.rows, input_image.cols, CV_8UC1);
		output_image_color = Mat::zeros(input_image.rows, input_image.cols, CV_8UC3);
		//convert RGB -> HSV
		cvtColor(input_image, input_image, CV_RGB2HSV);

		//could be done with INRANGE fuction.
		//range (input, min, max, output)

		//to get green: 
		//hmin = 70; hmax = 94
		//smin = 33; smax = 180
		//vmin = 113; vmax = 243
		inRange(input_image, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), output_image_gray);
	
		//pixel by pixel
		//for (int r = 0; r < input_image.rows; r++) {
		//	for (int c = 0; c < input_image.cols; c++) 
		//		
		//	}
		//}


		//set color X to Y
		output_image_color.setTo(Scalar(100, 200, 100), output_image_gray);
		imshow("webcam1", output_image_color);

		if (waitKey(1) == 27)
			break;
	}
	return(0);
}