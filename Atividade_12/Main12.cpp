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
	Mat object = imread("A.jpg");
	resize(object, object, Size(500, 500));
	//Mat object = imread("A.jpg");
	//resize(object, object, Size(300, 500));
	//declarar matrizes
	Mat image;
	Mat output_image;
	Mat desc1, desc2;

	//declaração de vetor que salva os keypoints (ver parâmetros salvos)
	vector<KeyPoint> keypoints_object, keypoints_image;
	Ptr<SURF>detector = SURF::create();
	//detectar features
	detector->detectAndCompute(object, Mat(), keypoints_object, desc1);

	VideoCapture video(0);
	if (!video.isOpened())
		return -1;

	while (video.read(image)) {
		//detectar features do frame
		detector->detectAndCompute(image, Mat(), keypoints_image, desc2);

		FlannBasedMatcher matcher;
		//declaração de vetor que salva os matches (ver parâmetros salvos)
		vector<DMatch> matches;
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

		//declaração de vetor que salva os matches confiáveis (ver parâmetros salvos)
		vector<DMatch> good_matches;

		for (int i = 0; i < desc1.rows; i++) {
			if (matches[i].distance <= max(2 * min_dist, 0.02))
				good_matches.push_back(matches[i]);
		}

		drawMatches(object, keypoints_object, image, keypoints_image, 
			good_matches, output_image, Scalar::all(-1), Scalar::all(-1),
			vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

		//Declaração de vetor de pontos da imagem (x, y)
		vector<Point2f> obj;
		vector<Point2f> scene;

		//salvando os keypoints dos good matches
		for (int i = 0; i < good_matches.size(); i++)
		{
			obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
			scene.push_back(keypoints_image[good_matches[i].trainIdx].pt);
		}

		//Finds a perspective transformation between two planes (pose)
		Mat H = findHomography(obj, scene, RANSAC);
		if (!H.empty()) {
			// Get the corners from the object ( the object to be "detected" )
			vector<Point2f> object_corners(4);
			object_corners[0] = Point(0, 0);
			object_corners[1] = Point(object.cols, 0);
			object_corners[2] = Point(object.cols, object.rows);
			object_corners[3] = Point(0, object.rows);

			//vector to save corners in the image
			vector<Point2f> image_corners(4);

			perspectiveTransform(object_corners, image_corners, H);

			// Draw lines between the corners (the mapped object in the scene)
			line(output_image, image_corners[0] + Point2f(object.cols, 0), image_corners[1] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
			line(output_image, image_corners[1] + Point2f(object.cols, 0), image_corners[2] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
			line(output_image, image_corners[2] + Point2f(object.cols, 0), image_corners[3] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
			line(output_image, image_corners[3] + Point2f(object.cols, 0), image_corners[0] + Point2f(object.cols, 0), Scalar(0, 255, 0), 4);
		}

		//-- Show detected matches
		imshow("Good Matches & Object detection", output_image);
		waitKey(1);
	}
	return 0;
}