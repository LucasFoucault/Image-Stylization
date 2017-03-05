 #ifndef PYRAMIDALSTROKERENDERER
#define PYRAMIDALSTROKERENDERER

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "IStrokeRenderer.h"
#include "IStroker.h"

using namespace cv;
using namespace std;

#define	CLAMP_int(a)	MAX(MIN(a,255),0)
#define	CLAMP_01(a)	    MAX(MIN(a,1),0)

class PyramidalStrokeRenderer : public IStrokeRenderer
{
private:
	int _levels;
	vector<IStroker*>& _strokers;
	vector<StrokeRenderer*>& _renderer;
public:
	PyramidalStrokeRenderer(const Mat& image, int levels, vector<IStroker*>& strokers, vector<StrokeRenderer*>& renderer);
	void Process(const Mat& image, Mat& output, const int* nbStroke);
	const float getPercent();
private:
	void ComputeGaussianPyramid(const Mat& img, vector<Mat>& GaussianPyramid);
protected:
	const float _percent;
};

#endif