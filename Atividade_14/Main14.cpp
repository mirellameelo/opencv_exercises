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

	Mat faceA = imread("A.jpg");
	resize(faceA, faceA, Size(500, 500));
	Mat faceD = imread("D.jpg");
	resize(faceD, faceD, Size(200, 500));
	Mat desc_scene, descA, descD;
	Mat output_image;
	Mat scene;

	vector<KeyPoint> keypoints_scene, keypoints_faceA, keypoints_faceD;
	Ptr<SIFT>detector = SIFT::create();

	//detector->detectAndCompute(scene, Mat(), keypoints_scene, desc_scene);
	detector->detectAndCompute(faceA, Mat(), keypoints_faceA, descA);
	detector->detectAndCompute(faceD, Mat(), keypoints_faceD, descD);


	vector<Point2d> keypoints_2D;
	vector<Point3d> keypoints_3D;
	for (int i = 0; i < keypoints_faceA.size(); i++) {
		keypoints_2D.push_back(keypoints_faceA[i].pt);
		keypoints_3D.push_back(Point3d(keypoints_2D[i].x, keypoints_2D[i].y, 0));
	}
	for (int i = 0; i < keypoints_faceD.size(); i++) {
		keypoints_2D.push_back(keypoints_faceD[i].pt);
		keypoints_3D.push_back(Point3d(0, keypoints_faceD[i].pt.y, 200-keypoints_faceD[i].pt.x));
	}

	Mat desc_total;
	vconcat(descA, descD, desc_total);

	VideoCapture video(0);
	if (!video.isOpened())
		return -1;

	while (video.read(scene)) {
		//Mat scene = imread("caixa.jpg");
		detector->detectAndCompute(scene, Mat(), keypoints_scene, desc_scene);

		//matchess
		FlannBasedMatcher matcher;

		//A
		vector< DMatch > matchess;
		matcher.match(desc_total, desc_scene, matchess);

		double max_dist = 0;
		double min_dist = 100;

		//calculation of max and min distances between keypoints
		for (int i = 0; i < desc_total.rows; i++) {
			double dist = matchess[i].distance;
			if (dist < min_dist)
				min_dist = dist;
			else if (dist > max_dist)
				max_dist = dist;
		}

		vector< DMatch > good_matches;
		for (int i = 0; i < desc_total.rows; i++) {
			if (matchess[i].distance <= max(2 * min_dist, 0.02))
				good_matches.push_back(matchess[i]);
		}

		vector<Point3d> matches_3D;
		for (int i = 0; i < good_matches.size(); i++) {
			//-- Get the keypoints from the good matches
			matches_3D.push_back(keypoints_3D[good_matches[i].queryIdx]);
		}

		vector<Point2d> matches_2D;
		for (int i = 0; i < good_matches.size(); i++) {
			//-- Get the keypoints from the good matches
			matches_2D.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
		}

		//Camera settings
		double focal_length = scene.cols; // Approximate focal length.
		Point2d center = Point2d(scene.cols / 2, scene.rows / 2);
		Mat camera_matrix = (Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
		// Assuming no lens distortion
		Mat dist_coeffs = Mat::zeros(4, 1, cv::DataType<double>::type);

		Mat rotation_vector;
		Mat translation_vector;
		Mat cam, rot;

		solvePnPRansac(matches_3D, matches_2D, camera_matrix, dist_coeffs,
			rotation_vector, translation_vector);

		//for (int i = 0; i < matches_2D.size(); i++) {
		//	circle(scene, matches_2D[i], 2, Scalar(0, 0, 255), -1);
		//}

		vector<Point2d> projection_2D, D_2D;

		vector<Point3d> A_3D;
		A_3D.push_back(Point3d(0, 0, 0)); //F-0
		A_3D.push_back(Point3d(0, 0, 200)); //E-1
		A_3D.push_back(Point3d(0, 500, 0)); //B-2
		A_3D.push_back(Point3d(500, 0, 0)); //G-3
		A_3D.push_back(Point3d(500, 500, 0)); //C-4
		A_3D.push_back(Point3d(0, 500, 200)); //A-5
		A_3D.push_back(Point3d(500, 0, 200)); //H-6
		A_3D.push_back(Point3d(500, 500, 200)); //D-7

		//projectPoints(matches_3D, rotation_vector, translation_vector,
		//	camera_matrix, dist_coeffs, projection_2D);

		projectPoints(A_3D, rotation_vector, translation_vector,
			camera_matrix, dist_coeffs, projection_2D);

		//for (int i = 0; i < projection_2D.size(); i++) {
		//	projection_2D[i].y = scene.rows - projection_2D[i].y;
		//}

		line(scene, projection_2D[0], projection_2D[1], cv::Scalar(255, 0, 0), 2); //z - blue //F-E
		line(scene, projection_2D[0], projection_2D[2], cv::Scalar(0, 255, 0), 2); //y - green //F-B
		line(scene, projection_2D[0], projection_2D[3], cv::Scalar(0, 0, 255), 2); //x - red //F-G
		line(scene, projection_2D[4], projection_2D[2], cv::Scalar(0, 0, 255), 2); //x - red //C-B
		line(scene, projection_2D[3], projection_2D[4], cv::Scalar(0, 200, 255), 2); //x - red //G-C
		line(scene, projection_2D[2], projection_2D[5], cv::Scalar(150, 200, 255), 2); //x - red //B-A
		line(scene, projection_2D[1], projection_2D[5], cv::Scalar(150, 200, 255), 2); //x - red //E-A
		line(scene, projection_2D[1], projection_2D[6], cv::Scalar(150, 200, 255), 2); //x - red //E-H
		line(scene, projection_2D[6], projection_2D[7], cv::Scalar(150, 200, 255), 2); //x - red //H-D
		line(scene, projection_2D[6], projection_2D[3], cv::Scalar(50, 100, 255), 2); //x - red //H-G
		line(scene, projection_2D[4], projection_2D[7], cv::Scalar(100, 255, 255), 2); //x - red //C-D
		line(scene, projection_2D[5], projection_2D[7], cv::Scalar(255, 255, 255), 2); //x - red //A-D
	
		//resize(scene, scene, Size(600, 600));
		imshow("mirella3", scene);
		waitKey(1);
		//system("pause");
	}
	return 0;


}
