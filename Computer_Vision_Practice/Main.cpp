#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <Windows.h>
#include "Lab7ParticleFilter.hpp"

using namespace cv;
using namespace std;

Mat image(600, 800, CV_8UC3);

#define drawCross( center, color, d )                                \
line( image, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, 0, 0); \
line( image, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, 0, 0 )


int main(int argc, char** argv) {

	vector<Point> mousev, particlev;
	mousev.clear();
	particlev.clear();

	ParticleFilter *pf = new ParticleFilter();

	POINT mousePos;
	GetCursorPos(&mousePos);

	pf = new ParticleFilter(200, mousePos.x, mousePos.y, 0, 0, 800, 600);
	pf->CalcWeight(mousePos.x, mousePos.y);

	while (1)
	{
		image = Mat(image.size(), CV_8UC3, Scalar(0, 0, 0));
		GetCursorPos(&mousePos);
		pf->Resampling();
		pf->Predict();
		pf->CalcWeight(mousePos.x, mousePos.y);
		Point pt = pf->Measure();
		double angle = 30;


		vector<Particle> particle_vector = pf->GetParticle();
		for (int i = 0; i < particle_vector.size(); i++)
		{
			ellipse(image, Point(particle_vector[i].x, particle_vector[i].y), Size(1, 1), angle, angle, angle + 360, Scalar(0, 200, 0), 5, 8);
		}

		ellipse(image, pt, Size(10, 10), angle, angle, angle + 360, Scalar(200, 0, 0), 5, 8);

		Point measPt(mousePos.x, mousePos.y);
		mousev.push_back(measPt);
		particlev.push_back(pt);
		drawCross(pt, Scalar(255, 255, 255), 5);
		drawCross(measPt, Scalar(0, 0, 255), 5);

		for (int i = 0; i < mousev.size() - 1; i++)
			line(image, mousev[i], mousev[i + 1], Scalar(255, 255, 0), 1);

		for (int i = 0; i < particlev.size() - 1; i++)
			line(image, particlev[i], particlev[i + 1], Scalar(0, 155, 255), 1);


		imshow("Camera", image);

		char c = (char)waitKey(80);
	}
}