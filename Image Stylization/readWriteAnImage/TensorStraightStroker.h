#ifndef TENSORSTRAIGHTSTROKER
#define TENSORSTRAIGHTSTROKER

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "TensorBasedStroker.h"

using namespace cv;
using namespace std;

class TensorStraightStroker : public TensorBasedStroker
{
public:
	TensorStraightStroker(int thickness, float decrement, bool grayScale = false);
	virtual void DrawLine(const Mat& original, Mat& image, Point point);
private:
	float _decrement;
	int _thickness;
	bool _blackAndWhite;
};

#endif