#ifndef ISTROKER
#define ISTROKER

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;
using namespace std;

class IStroker
{
public:
	virtual void PreProcess(const Mat& original, const float& percent) = 0;
	virtual void DrawLine(const Mat& original, Mat& image, Point point) = 0;

	vector<Point> _validPoints;
	vector<Vec2f> _validEigenValues;
	vector<Vec2f> _validEdgeNormals;
	vector<Vec2f> _validIsophotes;

protected:
	virtual void DefineTreshold(const float& percent) = 0;
	virtual void ListValidPoints(const float& percent) = 0;
};

#endif