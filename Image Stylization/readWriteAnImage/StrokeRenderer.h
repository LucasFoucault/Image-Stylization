#ifndef STROKERENDERER
#define STROKERENDERER

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

class StrokeRenderer : public IStrokeRenderer
{
public:
	StrokeRenderer(float percent, IStroker& stroker);
	void Process(const Mat& image, Mat& output, const int* nbStroke);
	const float getPercent();
private:
	IStroker& _stroker; 
protected:
	const float _percent;
};

#endif