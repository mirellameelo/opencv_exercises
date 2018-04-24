#include <stdio.h>
#include <iostream>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

//Load two images
//Detect keypoints in each image
//Extract SIFT features for those keypoints
//Find the closest matching features between the two images
//Display the images side - by - side and draw lines connecting the matching features


int main(int argc, char** argv) {

	//carregar imagem do objeto a ser detectado
	Mat im = imread("caixa.jpg");

	// 2D image points. If you change the image, you need to change vector
	std::vector<cv::Point2d> image_points;
	image_points.push_back(cv::Point2d(2049, 2213));    // A
	image_points.push_back(cv::Point2d(2469, 2621));    // B
	image_points.push_back(cv::Point2d(3621, 2141));   // C
	//image_points.push_back(cv::Point2d(3141, 1948));   // D
	image_points.push_back(cv::Point2d(2052, 737));     // E
	image_points.push_back(cv::Point2d(5168, 2907));     // F
	image_points.push_back(cv::Point2d(3865, 761));    // G
	image_points.push_back(cv::Point2d(3337, 625));     // H

	// 3D model points.
	vector<Point3d> model_points;
	model_points.push_back(Point3d(0.0f, 0.0f, 0.0f));       // A
	model_points.push_back(Point3d(0.0f, 0.0f, 6.5f));       // B
	model_points.push_back(Point3d(13.0f, 0.0f, 6.5f));      // C
	//model_points.push_back(Point3d(13.0f, 0.0f, 0.0f));      // D
	model_points.push_back(Point3d(0.0f, 13.0f, 0.0f));      // E
	model_points.push_back(Point3d(0.0f, 13.0f, 6.5f));      // F
	model_points.push_back(Point3d(13.0f, 13.0f, 6.5f));     // G
	model_points.push_back(Point3d(13.0f, 13.0f, 0.0f));     // H

	//Camera internals
	double focal_length = im.cols; // Approximate focal length.
	Point2d center = Point2d(im.cols / 2, im.rows / 2);
	Mat camera_matrix = (Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
	// Assuming no lens distortion
	Mat dist_coeffs = Mat::zeros(4, 1, cv::DataType<double>::type); 

	cout << "Camera Matrix " << endl << camera_matrix << endl;
	// Output rotation and translation
	cv::Mat rotation_vector; // Rotation in axis-angle form
	cv::Mat translation_vector;

	solvePnP(model_points, image_points, camera_matrix, dist_coeffs, 
		rotation_vector, translation_vector);

	// Project a 3D point (0, 0, 1000.0) onto the image plane.
	vector<Point3d> A_3D;
	vector<Point2d> A_2D;
	A_3D.push_back(Point3d(0, 0, 20.0));

	projectPoints(A_3D, rotation_vector, translation_vector,
		camera_matrix, dist_coeffs, A_2D);

	for (int i = 0; i < image_points.size(); i++)
	{
		circle(im, image_points[i], 20, Scalar(0, 0, 255), -1);
	}

	cv::line(im, image_points[0], A_2D[0], cv::Scalar(255, 0, 0), 2);

	cout << "Rotation Vector " << endl << rotation_vector << endl;
	cout << "Translation Vector" << endl << translation_vector << endl;

	cout << A_2D << endl;

	// Display image.
	resize(im, im, Size(800, 600));
	cv::imshow("Output", im);
	cv::waitKey(0);

}
