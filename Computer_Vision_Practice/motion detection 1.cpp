#include "stdafx.h"

#include "opencv2/opencv.hpp"

#include <iostream>
#include <sstream>


using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat frame;
	Mat background;
	Mat object;

	VideoCapture cap(argv[1]); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	cap.read(frame);
	Mat acc = Mat::zeros(frame.size(), CV_32FC3);

	namedWindow("frame", 1);
	namedWindow("Running Average");
	namedWindow("Threshold");
	for (;;)
	{
		Mat gray;
		cap >> frame; // get a new frame from camera
		imshow("frame", frame);

		// Get 50% of the new frame and add it to 50% of the accumulator
		accumulateWeighted(frame, acc, 0.05);

		// Scale it to 8-bit unsigned
		convertScaleAbs(acc, background);

		imshow("Original", frame);

		imshow("Weighted Average", background);

		subtract(frame, background, frame);
		threshold(frame, frame, 30, 255, 0);

		imshow("Threshold", frame);

		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}