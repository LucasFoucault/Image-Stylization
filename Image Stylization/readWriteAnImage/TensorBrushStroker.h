#ifndef TENSORBRUSHSTROKER
#define TENSORBRUSHSTROKER

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "TensorBasedStroker.h"

using namespace cv;
using namespace std;

class TensorBrushStroker : public TensorBasedStroker
{
public:
	TensorBrushStroker(Mat3f& brush);
	virtual void DrawLine(const Mat& original, Mat& image, Point point);
private:
	Mat3f& _brush;
};

#endif