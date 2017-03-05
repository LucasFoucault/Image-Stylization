#ifndef ISTROKERENDERER
#define ISTROKERENDERER

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;
using namespace std;

class IStrokeRenderer
{
public:
	virtual void Process(const Mat& image, Mat& result, const int* nbStroke) = 0;
	virtual const float getPercent() = 0;
};

#endif