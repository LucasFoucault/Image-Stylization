#ifndef ILOWPOLY
#define ILOWPOLY

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "IStroker.h"

using namespace cv;
using namespace std;

class ILowPoly
{
public:
	virtual void PreProcess(const Mat& original, IStroker& stroker, float& percent, const int* nbStroke) = 0;
	virtual void DrawPoly(const Mat& original, Mat& image, vector<Point>& pointList) = 0;
};

#endif