#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <random>

using namespace std;
using namespace cv;

class Particle
{
public:
	int x;
	int y;
	double weight;
	double act() const { return weight; }

	Particle(int x, int y);
	void PrintOut();
};

class ParticleFilter
{
private:
	int n_sample;
	int xmin, ymin, width, height;
	vector<Particle> particle_vector;
public:
	ParticleFilter();
	ParticleFilter(int particle_num, int x, int y, int xmin, int ymin, int width, int height);
	void Predict();
	void CalcWeight(int x, int y);
	void Resampling();
	Point Measure();

	vector<Particle> GetParticle();
};