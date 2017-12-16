#include "stdafx.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat src, dst;

	char* source_window = "Source image";
	char* equalized_window = "Equalized Image";

	/// Load image
	src = imread(argv[1], 1);

	if (!src.data)
	{
		cout << "Usage: ./Histogram_Demo <path_to_image>" << endl;
		return -1;
	}

	/// Convert to grayscale
	cvtColor(src, src, COLOR_BGR2GRAY);

	/// Apply Histogram Equalization
	equalizeHist(src, dst);

	/// Display results
	namedWindow(source_window, WINDOW_AUTOSIZE);
	namedWindow(equalized_window, WINDOW_AUTOSIZE);

	imshow(source_window, src);
	imwrite("elena_gray.jpg", src);
	imshow(equalized_window, dst);
	imwrite("elena_gray_equalization.jpg", dst);

	/// Wait until user exits the program
	waitKey(0);

	return 0;
}
