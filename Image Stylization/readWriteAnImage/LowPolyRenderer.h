#ifndef LOWPOLYRENDERER
#define LOWPOLYRENDERER

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "ILowPolyRenderer.h"
#include "TensorBasedLowPoly.h"

using namespace cv;
using namespace std;

class LowPolyRenderer : ILowPolyRenderer
{
public:
	LowPolyRenderer(TensorBasedLowPoly& lowPoly);
	virtual void Process(const Mat& original, Mat& result, const int type, VideoWriter video);
private:
	TensorBasedLowPoly& _lowPoly;
};

#endif