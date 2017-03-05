#ifndef TENSORBASEDLOWPOLY
#define TENSORBASEDLOWPOLY

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "ILowPoly.h"

using namespace cv;
using namespace std;

class TensorBasedLowPoly : public ILowPoly
{
public:
	virtual void PreProcess(const Mat& original, IStroker& stroker, float& percent, const int* nbStroke);
	virtual void DrawPoly(const Mat& original, Mat& image, vector<Point>& pointList) = 0;

	Subdiv2D _subdiv;
};

#endif