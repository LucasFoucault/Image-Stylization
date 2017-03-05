#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;
using namespace std;

#include "VoronoiDiagram.h"
#include "MatTools.h"

VoronoiDiagram::VoronoiDiagram()
{
}

void VoronoiDiagram::DrawPoly(const Mat& original, Mat& image, vector<Point>& pointList)
{
	const int npoints = pointList.size() - 1;
	const Point** points = new const Point*[npoints + 1];

	for (int j = 0; j<npoints; j++)
	{
		points[j] = &pointList[j];
	}

	// Create the mask with the polygon
	Mat1b mask(image.rows, image.cols, uchar(0));
	fillPoly(mask, points, &npoints, 1, Scalar(255));

	// Compute the color with the computed mask
	Scalar average = mean(original, mask);
	//Scalar average(original.at<Vec3b>(pointList[npoints]));

	// Complete the facet with mean color
	fillPoly(image, points, &npoints, 1, average);

	// Draw polygons completely inside the image.
	//for(int j=0; j<npoints; j++)
	//{
	//	if(j != npoints-1)
	//		line(image, *points[j], *points[j+1], average, 1, CV_AA, 0);
	//}

	delete[] points;
}