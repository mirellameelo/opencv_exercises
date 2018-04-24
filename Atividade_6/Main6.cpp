#include <iostream>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

int main(int argv, char** argc) {

	Mat image = imread("fire_man.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	Mat border = Mat::zeros(image.rows, image.cols, CV_8UC1);
	int max = 0;
	int mod, n;

	uchar *image_pointer = image.data;
	uchar *border_pointer = border.data;

	int t = image.cols*image.rows;

	//trackbar
	namedWindow("Range");
	createTrackbar("max", "Range", &max, 255);

	while (true) {
		for ( n = 1; n < t - 1; n++) {
			//keeping pixels in the right and left border
			if ( n % image.cols != 0 && (n+1) % image.cols != 0)
			{
				border_pointer[n] = abs(image_pointer[n + 1] - image_pointer[n - 1]);
				if (border_pointer[n] < max)
					border_pointer[n] = 0;
				else 
					border_pointer[n] = 255;
			}
		}
		imshow("result", border);
		if (waitKey(1) == 27)
			break;
	}

	waitKey();
	system("pause");

	return(0);
}