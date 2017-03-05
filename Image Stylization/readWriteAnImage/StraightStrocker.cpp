#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;
using namespace std;

#define	CLAMP_01(a)	    MAX(MIN(a,1),0)

#include "StraightStroker.h"

StraightStroker::StraightStroker(int thickness, float decrement, bool blackAndWhite)
	: _decrement(decrement), _thickness(thickness), _blackAndWhite(blackAndWhite)
{
}

void StraightStroker::DrawLine(const Mat& original, Mat& image, Point point)
{
	Vec2f gradient = _gradientOrientation.at<Vec2f>(point);
	Vec2f g = Vec2f(gradient[1], -gradient[0]);

	double length = norm(g);
	g = (length / 2) * (g / length); // coeff. 2 empirique

	if (abs(g[0]) > abs(g[1]))
		g /= g[0];
	else
		g /= g[1];

	g *= 20; // empirique

	Point2i pt1((int)(point.x-g[0]), (int)(point.y-g[1]));
	Point2i pt2((int)(point.x+g[0]), (int)(point.y+g[1]));

	int rows = image.rows;
	int cols = image.cols;

	LineIterator lineIterator(image, pt1, pt2, 8, true);
	
	int i;
	int count = lineIterator.count;
	static Mat visited(rows,cols,CV_8U);
	visited.setTo(0);

	Mat convertOriginal;
	original.convertTo(convertOriginal, CV_32FC3, 1.0f/255.0f);

	for(i = 0; i < count; i++, ++lineIterator)
	{
		int thickness = _thickness - 1;
		for(int x = -thickness; x <= thickness; x++)
		{
			for(int  y= -thickness; y <= thickness; y++)
			{
				Point pos = lineIterator.pos() + Point(x,y);
				if(pos.x >= 0 && pos.x < cols && pos.y >= 0 && pos.y < rows)
				{
					if(visited.at<unsigned char>(pos) == 0)
					{
						Vec3f& pix = image.at<Vec3f>(pos);

						const Vec3f& o = _blackAndWhite ? Vec3f(0.5, 0.5, 0.5) : convertOriginal.at<Vec3f>(pos);

						if(_decrement < 0)
							pix += (Vec3f(1.0f,1.0f,1.0f) - o) * _decrement;
						else
							pix += o * _decrement;

						pix[0] = CLAMP_01(pix[0]);
						pix[1] = CLAMP_01(pix[1]);
						pix[2] = CLAMP_01(pix[2]);

						visited.at<unsigned char>(pos) = (unsigned char)1;
					}
				}
			}
		}
	}
}