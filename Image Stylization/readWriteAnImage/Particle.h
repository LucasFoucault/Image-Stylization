#ifndef PARTICLE
#define PARTICLE

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "TensorStraightStroker.h";

using namespace cv;
using namespace std;

class Particle
{
public:
	Particle(int rows, int cols, float maxSpeed);

	void setUp(vector<Vec2f>& isophotes, TensorStraightStroker stroker, int interval, int rows, int cols, int choix);
	void update();
	void show(const Mat& original, Mat& image);
	void follow(vector<Point>& validPoints, vector<Vec2f>& vectors, int interval, int cols, int choix);
	void edges(int width, int height);
	
	void applyForce(Vec2f force);
	void updatePrev();
	void normalizeIsophote(vector<Vec2f>& isophotes, int index);

	int _cols, _rows;
	float _maxSpeed;

	Vec2f pos;
	Vec2f prePos;
	Vec2f vel;
	Vec2f acc;
	Scalar color;
	float thickness;
};

#endif