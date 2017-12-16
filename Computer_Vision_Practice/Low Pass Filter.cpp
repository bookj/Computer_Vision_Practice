#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 15;

Mat src_image1; Mat dst_image1;

Mat src1;
char window_name_[] = "Filter Demo 1";

/// Function headers
int display_caption(char* caption);
int display_dst(int delay);

/**
* function main
*/
int main(int argc, char** argv)
{
	namedWindow(window_name_, WINDOW_AUTOSIZE);

	/// Load the source image
	src_image1 = imread(argv[1]);

	if (display_caption("Original Image") != 0) { return 0; }

	dst_image1 = src_image1.clone();
	if (display_dst(DELAY_CAPTION) != 0) { return 0; }

	/// Applying Homogeneous blur
	if (display_caption("Homogeneous Blur") != 0) { return 0; }

	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		blur(src_image1, dst_image1, Size(i, i), Point(-1, -1));
		if (display_dst(DELAY_BLUR) != 0) { return 0; }
	}
	imwrite("blurr_homo.jpg", dst_image1);

	/// Applying Gaussian blur
	if (display_caption("Gaussian Blur") != 0) { return 0; }

	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		GaussianBlur(src_image1, dst_image1, Size(i, i), 0, 0);
		if (display_dst(DELAY_BLUR) != 0) { return 0; }
	}
	imwrite("blurr_gaussian.jpg", dst_image1);

	/// Applying Median blur
	if (display_caption("Median Blur") != 0) { return 0; }

	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		medianBlur(src_image1, dst_image1, i);
		if (display_dst(DELAY_BLUR) != 0) { return 0; }
	}
	imwrite("blurr_median.jpg", dst_image1);

	/// Applying Bilateral Filter
	if (display_caption("Bilateral Blur") != 0) { return 0; }

	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		bilateralFilter(src_image1, dst_image1, i, i * 2, i / 2);
		if (display_dst(DELAY_BLUR) != 0) { return 0; }
	}
	imwrite("blurr_bilateral.jpg", dst_image1);

	/// Wait until user press a key
	display_caption("End: Press a key!");

	waitKey(0);
	return 0;
}

int display_caption(char* caption)
{
	dst_image1 = Mat::zeros(src_image1.size(), src_image1.type());
	putText(dst_image1, caption,
		Point(src_image1.cols / 4, src_image1.rows / 2),
		FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));

	imshow(window_name_, dst_image1);
	int c = waitKey(DELAY_CAPTION);
	if (c >= 0) { return -1; }
	return 0;
}

int display_dst(int delay)
{
	imshow(window_name_, dst_image1);
	int c = waitKey(delay);
	if (c >= 0) { return -1; }
	return 0;
}