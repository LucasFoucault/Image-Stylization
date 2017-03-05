#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O
#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;
using namespace std;

#define	CLAMP_01(a)	    MAX(MIN(a,1),0)

#include "TensorBrushStroker.h"

TensorBrushStroker::TensorBrushStroker(Mat3f& brush)
	: _brush(brush)
{
}

void TensorBrushStroker::DrawLine(const Mat& original, Mat& image, Point point)
{
	int rows = image.rows;
	int cols = image.cols;

	// On calcule l'angle et l'échelle du path par rapport aux vecteurs et valeurs propres
	Vec2f isophote = _isophotes.at<Vec2f>(point);
    double angle = atan2(isophote[1], -isophote[0]) * 180 / M_PI;
	double scale = 1.0; // - length;

	// On fait une rotation de la brush pour en faire un patch
    Point2f center(_brush.cols / 2.0, _brush.rows / 2.0);
    Mat rot = getRotationMatrix2D(center, angle, scale);

	Rect bbox = RotatedRect(center, Size2f(_brush.cols * scale, _brush.rows * scale), angle).boundingRect();
    rot.at<double>(0,2) += bbox.width / 2.0 - center.x;
    rot.at<double>(1,2) += bbox.height / 2.0 - center.y;

    Mat3f patch;
	warpAffine(_brush, patch, rot, bbox.size(), INTER_LINEAR, BORDER_CONSTANT, cv::Scalar(255, 255, 255));

    Mat3f patchNorm;
	patch.convertTo(patchNorm, CV_32FC3, 1.0 / 255.0);

	for (int i = 0; i < patch.rows; i++)
	{
		int y = point.y - patch.rows / 2.0 + i;
		if (y < 0 || y >= rows)
			continue;

		for (int j = 0; j < patch.cols; j++)
		{
			int x = point.x - patch.cols / 2.0 + j;
			if (x < 0 || x >= cols)
				continue;
			
			Point patchPoint(j,i);
			Point imagePoint(x,y);
			Vec3f patchColor = patchNorm.at<Vec3f>(patchPoint);

			if (patchColor == Vec3f(0.0, 0.0, 0.0))
				continue;

			// On applique le patch pixel par pixel
			image.at<Vec3f>(imagePoint) -= Vec3f(1.0, 1.0, 1.0) - patchColor;
		}
	}
}