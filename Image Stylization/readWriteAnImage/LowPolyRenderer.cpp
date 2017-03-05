#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "LowPolyRenderer.h"
#include "MatTools.h"

using namespace cv;
using namespace std;

LowPolyRenderer::LowPolyRenderer(TensorBasedLowPoly& lowPoly)
	: _lowPoly(lowPoly)
{
}

void LowPolyRenderer::Process(const Mat& original, Mat& result, const int choix, VideoWriter video)
{
	vector< vector<Point> > pointList;

	if(choix == 1) // Delaunay
	{
		vector<Vec6f> triangleList;
		_lowPoly._subdiv.getTriangleList(triangleList);

		for(int i=0; i<triangleList.size(); i++)
		{
			Vec6f t = triangleList[i];
			vector<Point> pt;

			pt.push_back(Point(cvRound(t[0]), cvRound(t[1])));
			pt.push_back(Point(cvRound(t[2]), cvRound(t[3])));
			pt.push_back(Point(cvRound(t[4]), cvRound(t[5])));

			pointList.push_back(pt);
		}
	} 
	else if(choix == 2)	// Voronoi
	{
		vector<vector<Point2f> > facets;
		vector<Point2f> centers;
		_lowPoly._subdiv.getVoronoiFacetList(vector<int>(), facets, centers);
		
		pointList.resize(facets.size());

		for(int i=0; i<facets.size(); i++)
		{
			for(int j=0; j<facets[i].size(); j++)
				pointList[i].push_back(facets[i][j]);
			
			pointList[i].push_back(centers[i]);
		}
	}

	for(int i=0; i<pointList.size(); i++)
	{
		_lowPoly.DrawPoly(original, result, pointList[i]);

		imshow("IntermediateResults", MatTools::norm_0_255(result));
		waitKey(10);
		//video << result;
	}
}