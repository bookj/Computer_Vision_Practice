#include "stdafx.h"

#include "opencv2/opencv.hpp"

#include <iostream>
#include <sstream>


using namespace cv;
using namespace std;

Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor

int mainmd2(int argc, char** argv)
{
	VideoCapture cap(argv[1]); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	//create Background Subtractor objects
	pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach

	namedWindow("frame", 1);
	namedWindow("FG Mask MOG 2");
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		imshow("frame", frame);

		pMOG2->apply(frame, fgMaskMOG2);

		threshold(fgMaskMOG2, fgMaskMOG2, 250, 255, 0);

		imshow("FG Mask MOG 2", fgMaskMOG2);

		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}