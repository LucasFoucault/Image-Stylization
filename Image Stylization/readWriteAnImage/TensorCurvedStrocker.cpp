#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;
using namespace std;

#define	CLAMP_01(a)	    MAX(MIN(a,1),0)

#include "TensorCurvedStroker.h"

TensorCurvedStroker::TensorCurvedStroker(int thickness, float decrement, bool blackAndWhite, int keysCount, float keysMargin, int linesPerSubCurve)
	: _decrement(decrement), _thickness(thickness), _blackAndWhite(blackAndWhite), _keysCount(keysCount), _keysMargin(keysMargin), _linesPerSubCurve(linesPerSubCurve)
{
}

void TensorCurvedStroker::DrawLine(const Mat& original, Mat& image, Point point)
{
	Vec2f* keyPoints = new Vec2f[_keysCount * 2 + 1];
	Vec2f* tempPoints = new Vec2f[_keysCount];

	// On ajoute le point central à la liste de points
	keyPoints[_keysCount] = Vec2f(point.x, point.y);
	
	// On ajoute les points dans le sens de l'isophote
	GetKeyPoints(original, point, _keysCount, _keysMargin, tempPoints);
	for (int i = 0; i < _keysCount; i++)
		keyPoints[i] = tempPoints[_keysCount - 1 - i];
	
	// On ajoute les points dans le sens inverse de l'isophote
	GetKeyPoints(original, point, _keysCount, -_keysMargin, tempPoints);
	for (int i = 0; i < _keysCount; i++)
		keyPoints[_keysCount + 1 + i] = tempPoints[i];

	delete[] tempPoints;
	
	// Dessine les sous-courbe entre le second et l'avant-dernier point
	for (int i = 1; i < _keysCount * 2 + 1 - 2; i++)
		DrawCurve(original, image, keyPoints, i);

	delete[] keyPoints;
}

// Obtient N points par increment succesive dans le sens de l'isophote local
void TensorCurvedStroker::GetKeyPoints(const Mat& original, Point point, int count, float increment, Vec2f* result)
{
	Vec2f isophote;
	for (int i = 0; i < count; i++)
	{
		if (point.x >= 0 && point.y >= 0 && point.x < original.cols && point.y < original.rows)
		{
			Vec2f newIsophote = _isophotes.at<Vec2f>(point);
			if (norm(newIsophote) != 0)
			{
				isophote = newIsophote;
				isophote /= norm(isophote);
				isophote *= increment;
			}
		}

		result[i] = Vec2f(point.x + isophote[0], point.y + isophote[1]);
		point = Point(result[i][0], result[i][1]);
	}
}

// Dessine la spline Catmull-Rom entre 4 point de la liste autour de l'index i à l'aide d'un certain nombre de trait droit
void TensorCurvedStroker::DrawCurve(const Mat& original, Mat& image, Vec2f* keyPoints, int i)
{
	Vec2f p0 = keyPoints[i-1];
	Vec2f p1 = keyPoints[i];
	Vec2f p2 = keyPoints[i+1];
	Vec2f p3 = keyPoints[i+2];

	Vec2f previous = Vec2f(p1[0], p1[1]);
	for (int i = 1; i <= _linesPerSubCurve - 1; i++)
	{
		float t = (float)i / (float)_linesPerSubCurve;
		Vec2f next = CatmullRom(p0, p1, p2, p3, t);

		RenderLine(original, image, Point(previous[0], previous[1]), Point(next[0], next[1]));

		previous = next;
	}
	
	RenderLine(original, image, Point(previous[0], previous[1]), Point(p2[0], p2[1]));
}

void TensorCurvedStroker::RenderLine(const Mat& original, Mat& image, Point pt1, Point pt2)
{
	int rows = image.rows;
	int cols = image.cols;

	LineIterator lineIterator(image, pt1, pt2, 8, true);
	
	int i;
	int count = lineIterator.count;
	Mat visited(rows,cols,CV_8U);
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

// Génère la position en t sur une spline Catmull-Rom paramétré par 4 points.
// Une spline Catmull-Rom passe par p1 et p2
Vec2f TensorCurvedStroker::CatmullRom(Vec2f p0, Vec2f p1, Vec2f p2, Vec2f p3, float t)
{
	return 0.5f * ((2 * p1) + (-p0 + p2) * t + 
		   (2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t + 
		   (-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);
}