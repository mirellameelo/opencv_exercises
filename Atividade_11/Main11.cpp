#include <stdio.h>
#include <iostream>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

//Load two images
//Detect keypoints in each image
//Extract SIFT features for those keypoints
//Find the closest matching features between the two images
//Display the images side - by - side and draw lines connecting the matching features


int main(int argc, char** argv) {

	Mat object = imread("patinho.jpg");
	Mat image = imread("scene.jpg"); 
	cvtColor(object, object, COLOR_RGB2GRAY);
	cvtColor(image, image, COLOR_RGB2GRAY);
	Mat desc1, desc2;


	//detecting keypoints with SURF
	//input, features, mask
	Ptr<SURF>detector = SURF::create();
	vector<KeyPoint> keypoints_object, keypoints_image;

	detector->detectAndCompute(object, Mat(), keypoints_object, desc1);
	detector->detectAndCompute(image, Mat(), keypoints_image, desc2);


	//matching descriptor vectors using FLANN matcher
	//BFMatcher(). It takes two optional params. First one is normType. 
	//It specifies the distance measurement to be used. 
	//By default, it is cv2.NORM_L2. It is good for SIFT, SURF etc (cv2.NORM_L1 is also there). 
	//For binary string based descriptors like ORB, BRIEF, BRISK etc, cv2.NORM_HAMMING should be used, which used Hamming distance as measurement.
	FlannBasedMatcher matcher;
	vector< DMatch > matches;
	matcher.match(desc1, desc2, matches);

	double max_dist = 0;
	double min_dist = 100;

	//calculation of max and min distances between keypoints
	for (int i = 0; i < desc1.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
		else if (dist > max_dist)
			max_dist = dist;
	}

	vector< DMatch > good_matches;

	for (int i = 0; i < desc1.rows; i++) {
		if (matches[i].distance <= max(2 * min_dist, 0.02))
			good_matches.push_back(matches[i]);
	}

	//Drawing matches
	Mat output_image;

	drawMatches(object, keypoints_object, image, keypoints_image, good_matches, output_image,
		Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::DEFAULT);

	//drawMatches(object, keypoints_object, image, keypoints_image, matches, output_image);
	for (int i = 0; i < (int)good_matches.size(); i++)
	{
		printf("Keypoint 1: %d  -- Keypoint 2: %d  \n", good_matches[i].queryIdx, good_matches[i].trainIdx);
	}

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_image[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> object_corners(4);
	object_corners[0] = Point(0, 0); object_corners[1] = Point(object.cols, 0);
	object_corners[2] = Point(object.cols, object.rows); object_corners[3] = Point(0, object.rows);
	std::vector<Point2f> image_corners(4);

	perspectiveTransform(object_corners, image_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(output_image, image_corners[0] + Point2f(object.cols, 0), image_corners[1] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
	line(output_image, image_corners[1] + Point2f(object.cols, 0), image_corners[2] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
	line(output_image, image_corners[2] + Point2f(object.cols, 0), image_corners[3] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
	line(output_image, image_corners[3] + Point2f(object.cols, 0), image_corners[0] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);

	//-- Show detected matches
	imshow("Good Matches & Object detection", output_image);

	waitKey(0);
	return 0;
}
