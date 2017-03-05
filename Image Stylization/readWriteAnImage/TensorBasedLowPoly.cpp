#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;
using namespace std;

#include "TensorBasedLowPoly.h"
#include "MatTools.h"

void TensorBasedLowPoly::PreProcess(const Mat& original, IStroker& stroker, float& percent, const int* nbStroke)
{
	stroker.PreProcess(original, percent);

	Point* point = new Point[*nbStroke];

	RNG rng(0xFFFFFFFF);
	for (unsigned int i = 0; i < *nbStroke; i++)
	{
		// Tirage d'un point p sur le domaine de définition de l'image
		int index = rng.uniform(0, stroker._validPoints.size());

		point[i] = stroker._validPoints[index];
	}

	Size size = original.size();
	Rect rect(0, 0, size.width, size.height);
	Subdiv2D subdiv(rect);

	for (int i = 0; i < *nbStroke; i++)
	{
		subdiv.insert(point[i]);
	}

	_subdiv = subdiv;

	delete [] point;
}