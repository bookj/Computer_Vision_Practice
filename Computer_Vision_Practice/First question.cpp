#include "stdafx.h"

#include "opencv2/opencv.hpp"

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
//
//#define drawCross2( center, color, d )                                \
//line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, 0, 0); \
//line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, 0, 0 )

/** Function Headers */
void detectAndDisplay2(Mat frame);

/** Global variables */
String face_cascade_name2 = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name2 = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade2;
CascadeClassifier eyes_cascade2;
String window_name2 = "Capture - Face detection";

KalmanFilter KF(4, 2, 0);
Mat_<float> measurement(2, 1);
vector<Point> facecenter, kalmanv;

/** @function main */
int main(void)
{
	VideoCapture capture;
	Mat frame;

	//-- 1. Load the cascades
	if (!face_cascade2.load(face_cascade_name2)){ printf("--(!)Error loading face cascade\n"); return -1; };
	if (!eyes_cascade2.load(eyes_cascade_name2)){ printf("--(!)Error loading eyes cascade\n"); return -1; };

	//-- 2. Read the video stream
	capture.open(0);
	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }

	// intialization of KF...
	KF.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);

	KF.statePre.at<float>(0) = 0;
	KF.statePre.at<float>(1) = 0;
	KF.statePre.at<float>(2) = 0;
	KF.statePre.at<float>(3) = 0;

	setIdentity(KF.measurementMatrix);
	setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
	setIdentity(KF.measurementNoiseCov, Scalar::all(1e-3));
	setIdentity(KF.errorCovPost, Scalar::all(1e-4));

	measurement.setTo(Scalar(0));
	facecenter.clear();
	kalmanv.clear();

	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		//-- 3. Apply the classifier to the frame
		detectAndDisplay2(frame);

		int c = waitKey(10);
		if ((char)c == 27) { break; } // escape
	}
	return 0;
}

/** @function detectAndDisplay2 */
void detectAndDisplay2(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	face_cascade2.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// First predict, to update the internal statePre variable
	Mat prediction = KF.predict();
	Point predictPt(prediction.at<float>(0), prediction.at<float>(1));
	Mat estimated;

	for (size_t i = 0; i < faces.size(); i++)
	{
		int x, y, w, h;
		x = faces[i].x + faces[i].width / 2;
		y = faces[i].y + faces[i].height / 2;
		w = faces[i].width;
		h = faces[i].height;
		Point pt1(x - w / 2, y - h / 2);
		Point pt2(x + w / 2, y + h / 2);

		rectangle(frame, pt1, pt2, Scalar(0, 0, 255));

		// Get mouse point
		measurement(0) = x;
		measurement(1) = y;

		// The update phase 
		estimated = KF.correct(measurement);
	}

	Point statePt;
	Point measPt;
	if (faces.size() > 0)
	{
		statePt.x = estimated.at<float>(0);
		statePt.y = estimated.at<float>(1);
		measPt.x = measurement(0);
		measPt.y = measurement(1);
		facecenter.push_back(measPt);
		kalmanv.push_back(statePt);
	}
	else
	{
		statePt.x = prediction.at<float>(0);
		statePt.y = prediction.at<float>(1);
		kalmanv.push_back(statePt);
	}

	// plot points

	//drawCross2( statePt, Scalar(255,255,255), 5 );
	//drawCross2( measPt, Scalar(0,0,255), 5 );

	for (int i = 2; i < facecenter.size() - 1; i++)
		line(frame, facecenter[i], facecenter[i + 1], Scalar(0, 255, 0), 1);

	for (int i = 2; i < kalmanv.size() - 1; i++)
		line(frame, kalmanv[i], kalmanv[i + 1], Scalar(0, 0, 255), 1);

	//-- Show what you got
	imshow(window_name2, frame);
}
