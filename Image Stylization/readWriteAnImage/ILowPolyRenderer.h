#ifndef ILOWPOLYRENDERER
#define ILOWPOLYRENDERER

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

//#include "main.cpp"

using namespace cv;
using namespace std;

class ILowPolyRenderer
{
public:
	virtual void Process(const Mat& original, Mat& result, const int type, VideoWriter video) = 0;
};

#endif