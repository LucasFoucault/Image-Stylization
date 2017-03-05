#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;
using namespace std;

#include "Particle.h"
#include "MatTools.h"

Particle::Particle(int cols, int rows, float maxSpeed): _cols(cols), _rows(rows), _maxSpeed(maxSpeed)
{
	int rngCols = std::rand() % _cols + 1;
	int rngRows = std::rand() % _rows + 1;

	pos = Vec2f(rngCols, rngRows);
	prePos = pos;

	vel = Vec2f(0.0f, 0.0f);
	acc = Vec2f(0.0f, 0.0f);

	maxSpeed = 1.0f;
	color = Scalar(0,0,0);
	thickness = 1.0f;
}

void Particle::setUp(vector<Vec2f>& isophotes, TensorStraightStroker stroker, int interval, int rows, int cols, int choix)
{
	if (choix == 1)
	{
		for(int i=0; i<rows; i++)
			for(int j=0; j<cols; j++)
			{
				isophotes.push_back(stroker._validIsophotes[j*interval + i*interval*_cols]);

				int indexIso = j + i*cols;
				normalizeIsophote(isophotes, indexIso);
			}
	}
	else if (choix == 2)
	{
		int indexImage = 0;
		for (int i = 0; i<_rows; i++)
			for (int j = 0; j<_cols; j++)
			{
				Point point = stroker._validPoints[indexImage];

				if (point.x == j && point.y == i)
				{
					isophotes.push_back(stroker._validIsophotes[indexImage]);

					int indexIso = j + i*_cols;
					normalizeIsophote(isophotes, indexIso);

					indexImage++;
				}
				else
					isophotes.push_back(Vec2f(0.0f, 0.0f));
			}
	}
}

void Particle::update()
{
	vel[0] += acc[0];
	vel[1] += acc[1];

	// Limit vector vel to his maxSpeed
	float mSq = (vel[0] * vel[0]) + (vel[1] * vel[1]);
	if (mSq > _maxSpeed*_maxSpeed)
	{
		vel[0] /= sqrt(mSq);
		vel[1] /= sqrt(mSq);

		vel[0] *= _maxSpeed;
		vel[1] *= _maxSpeed;
	}

	pos[0] += vel[0];
	pos[1] += vel[1];

	acc[0] *= 0.0f;
	acc[1] *= 0.0f;
}

void Particle::show(const Mat& original, Mat& image)
{
	Point pt1(pos[0], pos[1]);
	Point pt2(prePos[0], prePos[1]);

	// COMMENT TO CREATE POP ART
		color = Scalar(original.at<Vec3b>(pt2));

	// UNCOMMENT TO CREATE POP ART AND INITIALIZE thickness AT 100.0f MINIMUM IN THE CONSTRUCTOR
		//thickness -= 1.0f;
		//if (thickness < 1.0f)
		//	thickness = 1.0f;

		//color[0] += 10;
		//color[1] += 20;
		//color[2] += 30;

		//if (color[0] > 255)
		//	color[0] = 0;

		//if (color[1] > 255)
		//	color[1] = 0;

		//if (color[2] > 255)
		//	color[2] = 0;

		//if (thickness == 1.0f)
		//{
		//	//color = Scalar(original.at<Vec3b>(pt2));
		//	//color[0] = std::rand() % 255 + 1;
		//	//color[1] = std::rand() % 255 + 1;
		//	//color[2] = std::rand() % 255 + 1;

		//	color[0] = 255;
		//	color[1] = 255;
		//	color[2] = 255;
		//}

	if (vel == Vec2f(0.0f, 0.0f))
	{
		int rngCols = std::rand() % _cols + 1;
		int rngRows = std::rand() % _rows + 1;

		pos = Vec2f(rngCols, rngRows);
		updatePrev();

		//if(thickness == 1.0f)
		//	return;
	}

	line(image, pt1, pt2, color, thickness, CV_AA);
	updatePrev();
}

void Particle::follow(vector<Point>& validPoints, vector<Vec2f>& flowFiedl, int interval, int cols, int choix)
{
	if (choix == 1)
	{
		int x = floor(pos[0] / interval);
		int y = floor(pos[1] / interval);

		int index = x + y * cols;

		applyForce(flowFiedl[index]);
	}
	else if (choix == 2)
	{
		int x = floor(pos[0]);
		int y = floor(pos[1]);

		int index = x + y * _cols;

		if (flowFiedl[index] == Vec2f(0.0f, 0.0f))
		{
			vel[0] *= 0.0f;
			vel[1] *= 0.0f; 
		}

		applyForce(flowFiedl[index]);
	}
}

void Particle::edges(int width, int height)
{
	if (pos[0] >= (float)width) {
		pos[0] = 0.0f;
		updatePrev();
	}
	if (pos[0] < 0.0f) {
		pos[0] = (float)width - 1;
		updatePrev();
	}
	if (pos[1] >= (float)height) {
		pos[1] = 0.0f;
		updatePrev();
	}
	if (pos[1] < 0.0f) {
		pos[1] = (float)height - 1;
		updatePrev();
	}
}

void Particle::applyForce(Vec2f force)
{
	acc[0] += force[0];
	acc[1] += force[1];
}

void Particle::updatePrev()
{
	prePos[0] = pos[0];
	prePos[1] = pos[1];
}

void Particle::normalizeIsophote(vector<Vec2f>& isophotes, int index)
{
	double norm = sqrt((isophotes[index][0] * isophotes[index][0]) + (isophotes[index][1] * isophotes[index][1]));

	if (norm == 0.0)
	{
		isophotes[index][0] = 0.0f;
		isophotes[index][1] = 0.0f;
	}
	else
	{
		isophotes[index][0] /= norm;
		isophotes[index][1] /= norm;
	}
}