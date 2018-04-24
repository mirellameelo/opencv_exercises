#include <iostream>
#include <stdint.h>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;
using namespace std;

int main() {

	Mat image = imread("disney_ruido.png", CV_LOAD_IMAGE_GRAYSCALE);
	resize(image, image, Size(image.cols / 2, image.rows / 2));
	imshow("grayscale", image);
	Mat output_image;
	//structure to erode and dilate
	Mat st = getStructuringElement(MORPH_RECT, Size(3, 3));

	for (int r = 0; r < image.rows; r++) {
		for (int c = 0; c < image.cols; c++) {
			if (image.at<uchar>(r, c) < 128)
				image.at<uchar>(r, c) = 255;
			else
				image.at<uchar>(r, c) = 0;
		}
	}



	imshow("binarizada", image);

	//erote
	erode(image, output_image, st);
	imshow("erosion", output_image);
	//dilate
	dilate(output_image, output_image, st);
	imshow("dalated", output_image);

	//blur
	GaussianBlur(image, output_image, Size(9, 9), 1, 10);
	imshow("blur", output_image);

	/*Mat filter = output_image * (1.0 / 255.0);

	output_image = image * filter;
	imshow("mult", output_image);
*/
	waitKey();
	return 0;
}