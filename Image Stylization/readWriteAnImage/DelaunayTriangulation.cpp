#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;
using namespace std;

#include "DelaunayTriangulation.h"
#include "MatTools.h"

DelaunayTriangulation::DelaunayTriangulation()
{
}

void DelaunayTriangulation::DrawPoly(const Mat& original, Mat& image, vector<Point>& pointList)
{   
	const Point* points[3] = {&pointList[0], &pointList[1], &pointList[2]};
	int npoints = 3;

	Size size = original.size();
	Rect rect(0, 0, size.width, size.height);

	if (rect.contains(pointList[0]) && rect.contains(pointList[1]) && rect.contains(pointList[2]))
	{
		// Compute triangle center
		Point center;
		center.x = (pointList[0].x + pointList[1].x + pointList[2].x) / 3.0f;
		center.y = (pointList[0].y + pointList[1].y + pointList[2].y) / 3.0f;

		// Get the center color of the triangle
		Scalar average(original.at<Vec3b>(center));

		// Fill the triangle
		fillPoly(image, points, &npoints, 1, average);

		// Draw rectangles completely inside the image.
		//if (rect.contains(pointList[0]) && rect.contains(pointList[1]) && rect.contains(pointList[2]))
		//{
		//	line(image, pointList[0], pointList[1], 1.0f, 1, CV_AA, 0);
		//	line(image, pointList[1], pointList[2], 1.0f, 1, CV_AA, 0);
		//	line(image, pointList[2], pointList[0], 1.0f, 1, CV_AA, 0);
		//}
	}
	else
	{
		//// Create the mask with the polygon
		Mat1b mask(image.rows, image.cols, uchar(0));
		fillPoly(mask, points, &npoints, 1, Scalar(255));

		// Compute the mean with the computed mask
		Scalar average = mean(original, mask);

		// Fill the triangle
		fillPoly(image, points, &npoints, 1, average);
	}
}