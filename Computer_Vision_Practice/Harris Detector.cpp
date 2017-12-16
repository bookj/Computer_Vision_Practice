#include "stdafx.h"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/// Global variables
Mat src_harris, src_gray_harris;
int thresh_harris = 200;
int max_thresh_harris = 255;

const char* source_window = "Source image";
const char* corners_window = "Corners detected";

/// Function header
void cornerHarris_demo(int, void*);

/**
* @function main
*/
int main41(int, char** argv)
{
	/// Load source image and convert it to gray
	src_harris = imread(argv[1], IMREAD_COLOR);
	cvtColor(src_harris, src_gray_harris, COLOR_BGR2GRAY);

	/// Create a window and a trackbar
	namedWindow(source_window, WINDOW_AUTOSIZE);
	createTrackbar("Threshold: ", source_window, &thresh_harris, max_thresh_harris, cornerHarris_demo);
	imshow(source_window, src_harris);

	cornerHarris_demo(0, 0);

	waitKey(0);
	return(0);
}

/**
* @function cornerHarris_demo
* @brief Executes the corner detection and draw a circle around the possible corners
*/
void cornerHarris_demo(int, void*)
{

	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros(src_harris.size(), CV_32FC1);

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	/// Detecting corners
	cornerHarris(src_gray_harris, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

	/// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);

	/// Drawing a circle around corners
	for (int j = 0; j < dst_norm.rows; j++)
	{
		for (int i = 0; i < dst_norm.cols; i++)
		{
			if ((int)dst_norm.at<float>(j, i) > thresh_harris)
			{
				circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
			}
		}
	}
	/// Showing the result
	namedWindow(corners_window, WINDOW_AUTOSIZE);
	imshow(corners_window, dst_norm_scaled);
}