#ifndef TENSORCURVEDSTROKER
#define TENSORCURVEDSTROKER

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "TensorBasedStroker.h"

using namespace cv;
using namespace std;

class TensorCurvedStroker : public TensorBasedStroker
{
public:
	TensorCurvedStroker(int thickness, float decrement, bool grayScale = false, int keysCount = 3, float keysMargin = 10, int linesPerSubCurve = 4);
	virtual void DrawLine(const Mat& original, Mat& image, Point point);
private:
	float _decrement;
	int _thickness;
	bool _blackAndWhite;
	int _keysCount;
	float _keysMargin;
	int _linesPerSubCurve;
	
	void GetKeyPoints(const Mat& original, Point point, int count, float increment, Vec2f* result);
	void DrawCurve(const Mat& original, Mat& image, Vec2f* keyPoints, int i);
	void RenderLine(const Mat& original, Mat& image, Point pt1, Point pt2);
	Vec2f CatmullRom(Vec2f p0, Vec2f p1, Vec2f p2, Vec2f p3, float t);
};

#endif