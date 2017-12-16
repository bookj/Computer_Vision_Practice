#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <sstream>
#include <conio.h>
#include <windows.h>

#include "Lab7ParticleFilter.hpp"

Particle::Particle(int x_point, int y_point)
{
	y = y_point;
	x = x_point;
}

void Particle::PrintOut()
{
	cout << "y:" << y << endl;
	cout << "x:" << x << endl;
}

ParticleFilter::ParticleFilter()
{

}

ParticleFilter::ParticleFilter(int particle_num, int x, int y, int xmin_, int ymin_, int width_, int height_)
{
	n_sample = particle_num;
	xmin = xmin_;
	ymin = ymin_;
	width = width_;
	height = height_;

	for (int t = 0; t < n_sample; t++)
	{
		Particle particle(x, y);
		particle_vector.push_back(particle);
	}
}

void ParticleFilter::Predict()
{
	for (int index = 0; index < n_sample; index++)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> score(-50.0, 50.0);
		int n[2];
		n[0] = score(mt) + 1;
		n[1] = score(mt) + 1;

		particle_vector[index].x += n[0];
		particle_vector[index].y += n[1];

		if (particle_vector[index].x < xmin) particle_vector[index].x = xmin;
		if (particle_vector[index].y < ymin) particle_vector[index].y = ymin;

		if (particle_vector[index].x >= width) particle_vector[index].x = width;
		if (particle_vector[index].y >= height) particle_vector[index].y = height;
	}
}

void ParticleFilter::CalcWeight(int x, int y)
{
	double sum_weight = 0.0, weight = 0;

	for (int index = 0; index < particle_vector.size(); index++)
	{
		int xk = particle_vector[index].x;
		int yk = particle_vector[index].y;

		double weight = sqrt(pow(xk - x, 2) + pow(yk - y, 2));

		particle_vector[index].weight = weight;
		sum_weight += weight;
	}

	weight = 0;
	for (int index = 0; index < particle_vector.size(); index++)
	{
		particle_vector[index].weight /= sum_weight;
		particle_vector[index].weight = 1 - particle_vector[index].weight;
		weight += particle_vector[index].weight;
	}

	for (int index = 0; index < particle_vector.size(); index++)
	{
		particle_vector[index].weight /= weight;
	}
}

bool cmp(Particle const & L, Particle const & R) {
	return L.act() > R.act();
}

void ParticleFilter::Resampling()
{
	sort(particle_vector.begin(), particle_vector.end(), cmp);

	vector<double> sum_weights(n_sample);
	sum_weights[0] = particle_vector[0].weight;
	for (int index = 1; index < n_sample; index++)
	{
		sum_weights[index] = sum_weights[index - 1] + particle_vector[index].weight;
	}

	vector<Particle> copy_particle_vector(particle_vector);
	cout << "sum_weight " << sum_weights[n_sample - 1] << endl;

	double weight_threshold = 0.5;
	// double weight_threshold = (double)(rand() % 10000) / 10000.0;
	for (int index = 0; index < n_sample; index++)
	{
		//for (int k = 0; k < n_sample; k++)
		{
			if (sum_weights[index] < weight_threshold)
			{
				continue;
			}
			else
			{
				particle_vector[index] = copy_particle_vector[0];
				particle_vector[index].weight = 0.0;
				//break;
			}
		}
	}
}

Point ParticleFilter::Measure()
{
	double x = 0.0;
	double y = 0.0;
	double weight = 0.0;
	for (int index = 0; index < n_sample; index++)
	{
		x += particle_vector[index].x * particle_vector[index].weight;
		y += particle_vector[index].y * particle_vector[index].weight;
	}

	Point pt;
	pt.x = x;
	pt.y = y;

	cout << pt << endl;

	return pt;
}

vector<Particle> ParticleFilter::GetParticle()
{
	return particle_vector;
}
