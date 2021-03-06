#include "stdafx.h"

#include "opencv2/opencv.hpp"

#include <iostream>
#include <sstream>


using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	RNG rng(12345);
	Mat fgObj; //fg mask fg mask generated by MOG2 method
	Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	VideoCapture cap(argv[1]); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	//create Background Subtractor objects
	pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach


	Mat elementErosion;
	elementErosion = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));


	namedWindow("frame", 1);
	namedWindow("Object");
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		imshow("frame", frame);

		pMOG2->apply(frame, fgObj);

		threshold(fgObj, fgObj, 250, 255, 0);

		erode(fgObj, fgObj, elementErosion);
		dilate(fgObj, fgObj, elementErosion);

		imshow("Object", fgObj);

		findContours(fgObj, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		vector<vector<Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		vector<Point2f>center(contours.size());
		vector<float>radius(contours.size());

		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
			minEnclosingCircle(contours_poly[i], center[i], radius[i]);
		}

		Mat drawing = Mat::zeros(fgObj.size(), CV_8UC3);
		for (size_t i = 0; i< contours.size(); i++)
		{
			if ((boundRect[i].width + boundRect[i].height) / 2 > 50)
			{
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(drawing, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point());
				rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
				circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);
			}
		}
		namedWindow("Contours", WINDOW_AUTOSIZE);
		imshow("Contours", drawing);

		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}