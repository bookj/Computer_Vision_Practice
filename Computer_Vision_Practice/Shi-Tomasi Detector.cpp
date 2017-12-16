#include "stdafx.h"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

Mat src_tomasi, src_gray_tomasi;
int maxCorners = 23;
int maxTrackbar = 100;
RNG rng_tomasi(12345);
const char* source_window_tomasi = "Image";

void goodFeaturesToTrack_Demo(int, void*);

int main(int, char** argv)
{
	src_tomasi = imread(argv[1], IMREAD_COLOR);
	cvtColor(src_tomasi, src_gray_tomasi, COLOR_BGR2GRAY);
	namedWindow(source_window_tomasi, WINDOW_AUTOSIZE);
	createTrackbar("Max  corners:", source_window_tomasi, &maxCorners, maxTrackbar, goodFeaturesToTrack_Demo);
	imshow(source_window_tomasi, src_tomasi);
	goodFeaturesToTrack_Demo(0, 0);
	waitKey(0);
	return(0);
}

void goodFeaturesToTrack_Demo(int, void*)
{
	if (maxCorners < 1) { maxCorners = 1; }
	vector<Point2f> corners;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	Mat copy;
	copy = src_tomasi.clone();
	goodFeaturesToTrack(src_gray_tomasi,
		corners,
		maxCorners,
		qualityLevel,
		minDistance,
		Mat(),
		blockSize,
		useHarrisDetector,
		k);
	cout << "** Number of corners detected: " << corners.size() << endl;
	int r = 4;
	for (size_t i = 0; i < corners.size(); i++)
	{
		circle(copy, corners[i], r, Scalar(rng_tomasi.uniform(0, 255), rng_tomasi.uniform(0, 255), rng_tomasi.uniform(0, 255)), -1, 8, 0);
	}
	namedWindow(source_window_tomasi, WINDOW_AUTOSIZE);
	imshow(source_window_tomasi, copy);
}