#ifndef DELAUNAYTRIANGULATION
#define DELAUNAYTRIANGULATION

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "TensorBasedLowPoly.h"

using namespace cv;
using namespace std;

class DelaunayTriangulation : public TensorBasedLowPoly
{
public:
	DelaunayTriangulation();

	virtual void DrawPoly(const Mat& original, Mat& image, vector<Point>& pointList);
};

#endif