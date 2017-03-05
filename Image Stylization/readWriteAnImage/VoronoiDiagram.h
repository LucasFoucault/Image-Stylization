#ifndef VORONOIDIAGRAM
#define VORONOIDIAGRAM

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "TensorBasedLowPoly.h"

using namespace cv;
using namespace std;

class VoronoiDiagram : public TensorBasedLowPoly
{
public:
	VoronoiDiagram();

	virtual void DrawPoly(const Mat& original, Mat& image, vector<Point>& pointList);
};

#endif