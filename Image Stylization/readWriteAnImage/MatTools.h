#ifndef MATTOOLS
#define MATTOOLS

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;

class MatTools
{
	public:
	static Mat MatTools::norm_0_255(InputArray _src)
	{
		Mat src = _src.getMat();

		// Create and return normalized image:
		Mat dst;
		switch (src.channels())
		{
			case 1:
				normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
				break;
			case 3:
				normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
				break;
			default:
				src.copyTo(dst);
				break;
		}
		return dst;
	}

	static Mat MatTools::norm_0_1(InputArray _src)
	{
		Mat src = _src.getMat();

		// Create and return normalized image:
		Mat dst;
		switch (src.channels())
		{
			case 1:
				normalize(_src, dst, 0.0f, 1.0f, NORM_MINMAX, CV_32FC1);
				break;
			case 3:
				normalize(_src, dst, 0.0f, 1.0f, NORM_MINMAX, CV_32FC3);
				break;
			default:
				src.copyTo(dst);
				break;
		}
		return dst;
	}

	static Mat MatTools::gaussianRegularization(Mat grad, int kernel)
	{
		Mat dst;
		
		// Size condition for Gaussian kernel
		if (kernel % 2 == 0) kernel++;
		if (kernel == 1) kernel = 3;
		
		// Deviation
		float a = (kernel-1)/4.0f;
		GaussianBlur(grad, dst, Size(kernel,kernel), a, a);

		return dst;
	}
};

#endif