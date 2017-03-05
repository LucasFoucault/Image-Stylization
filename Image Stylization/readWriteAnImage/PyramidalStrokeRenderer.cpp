#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O
#include <math.h>

#include "StrokeRenderer.h"
#include "PyramidalStrokeRenderer.h"

using namespace cv;
using namespace std;

#define	CLAMP_int(a)	MAX(MIN(a,255),0)
#define	CLAMP_01(a)	    MAX(MIN(a,1),0)

PyramidalStrokeRenderer::PyramidalStrokeRenderer(const Mat& image, int levels, vector<IStroker*>& strokers, vector<StrokeRenderer*>& renderer)
	: _levels(levels), _strokers(strokers), _renderer(renderer), _percent(0.0f)
{
	assert(_levels >= 1);
	assert(_levels == strokers.size());
}

void PyramidalStrokeRenderer::Process(const Mat& image, Mat& output, const int* nbStroke)
{
	vector<Mat> gaussianPyramid;
	ComputeGaussianPyramid(image, gaussianPyramid);

	// Initialize lower resolution according level
	output.rows = output.rows/pow(2.0f,_levels-1);
	output.cols = output.cols/pow(2.0f,_levels-1);

	// Display upsample stroke images
	for (int i = _levels - 1; i >= 0; i--)
	{
		// Calcul des gradients / tenseurs de l'image
		_strokers[i]->PreProcess(gaussianPyramid[i], _renderer[i]->getPercent());
		_renderer[i]->Process(gaussianPyramid[i], output, &nbStroke[i]);

		cout << "Level = " << i << endl;

		namedWindow("IntermediateResults", WINDOW_AUTOSIZE); // Create a window for display.
		imshow("IntermediateResults", output);

		namedWindow("ImageSource", WINDOW_AUTOSIZE); // Create a window for display.
		imshow("ImageSource", gaussianPyramid[i]);
		waitKey(100000);

		if(i != 0) pyrUp(output, output);
	}
}


void PyramidalStrokeRenderer::ComputeGaussianPyramid(const Mat& img, vector<Mat>& GaussianPyramid)
{
    Mat imgCopy;
    img.copyTo(imgCopy);

	// Create list of downsample image source
	for (int i = 0; i < _levels; i++)
	{
		GaussianPyramid.push_back(imgCopy);
        pyrDown(imgCopy, imgCopy);
	}
}

const float PyramidalStrokeRenderer::getPercent()
{ return _percent; }