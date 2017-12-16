#include "stdafx.h"

#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2/video/tracking.hpp"
#include <vector>
#include <stdio.h>
#include <Windows.h>
#include <iostream>


using namespace cv;
using namespace std;

int mainopt2(int argc, const char** argv)
{
	VideoCapture cap;

	Mat flow, frame;

	// some faster than mat image container
	UMat  flowUmat, prevgray;

	cv::CommandLineParser parser(argc, argv, "{@input|0|}");
	string input = parser.get<string>("@input");

	if (input.size() == 1 && isdigit(input[0]))
		cap.open(input[0] - '0');
	else
		cap.open(input);

	if (!cap.isOpened())
	{
		cout << "Could not initialize capturing...\n";
		return 0;
	}

	namedWindow("Optical Flow Field", WINDOW_AUTOSIZE);

	for (;;)
	{
		Mat img;
		Mat original;

		// capture frames
		cap >> frame;
		if (frame.empty())
			break;

		// capture frame from video file
		cap.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);
		resize(img, img, Size(320, 240));

		// save original for later
		img.copyTo(original);

		// just make current frame gray
		cvtColor(img, img, COLOR_BGR2GRAY);

		if (prevgray.empty() == false)
		{
			// calculate optical flow 
			calcOpticalFlowFarneback(prevgray, img, flowUmat, 0.4, 1, 12, 2, 8, 1.2, 0);
			// copy Umat container to standard Mat
			flowUmat.copyTo(flow);

			// By y += 5, x += 5 you can specify the grid 
			for (int y = 0; y < original.rows; y += 20)
			{
				for (int x = 0; x < original.cols; x += 20)
				{
					// get the flow from y, x position * 10 for better visibility
					const Point2f flowatxy = flow.at<Point2f>(y, x);

					// draw line at flow direction
					line(original, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(0, 255, 0));
					// draw initial point
					circle(original, Point(x, y), 1, Scalar(0, 0, 0), -1);
				}
			}

			// draw the results
			imshow("Optical Flow Field", original);

			// fill previous image again
			img.copyTo(prevgray);
		}
		else
		{
			// fill previous image in case prevgray.empty() == true
			img.copyTo(prevgray);
		}
		waitKey(10);
	}
}