#ifndef TENSORBASEDSTROKER
#define TENSORBASEDSTROKER

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "IStroker.h"

using namespace cv;
using namespace std;

class TensorBasedStroker : public IStroker
{
public:
	virtual void PreProcess(const Mat& original, const float& percent);
	virtual void DrawLine(const Mat& original, Mat& image, Point point) = 0;

protected:
	virtual void DefineTreshold(const float& percent);
	virtual void ListValidPoints(const float& percent);

	Mat _eigenValues;
	Mat _edgeNormals;
	Mat _isophotes;
	Mat _normValue;

	float _threshold;
};

#endif