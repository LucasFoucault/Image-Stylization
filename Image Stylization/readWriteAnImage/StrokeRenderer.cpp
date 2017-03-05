#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "StrokeRenderer.h"
#include "MatTools.h"

using namespace cv;
using namespace std;

#define	CLAMP_int(a)	MAX(MIN(a,255),0)
#define	CLAMP_01(a)	    MAX(MIN(a,1),0)

StrokeRenderer::StrokeRenderer(float percent, IStroker& stroker)
	: _stroker(stroker), _percent(percent)
{
}

void StrokeRenderer::Process(const Mat& image, Mat& output, const int* nbStroke)
{
	// Générateur de nombre aléatoire
	RNG rng(0xFFFFFFFF);

	// Création de la fenêtre de rendu intermédiaire
	// namedWindow("IntermediateResults", WINDOW_AUTOSIZE);

	// Boucle pour tracer les traits
	for (unsigned int i = 0; i < *nbStroke; i++)
	{	
		// Tirage d'un point p sur le domaine de définition de l'image
		int index = rng.uniform(0,_stroker._validPoints.size());
		Point p = _stroker._validPoints[index];
			
		//Vec3f pointColor = output.at<Vec3f>(p);
		//float pointNorm = sqrt(pow(pointColor[0], 2) + pow(pointColor[1], 2) + pow(pointColor[2], 2));

		//if(pointNorm > 0.5f)
			
		_stroker.DrawLine(image, output, p);

		// Affiche les résultats intermédiaires
		imshow("IntermediateResults", MatTools::norm_0_255(output));
		waitKey(10);
	}
}

const float StrokeRenderer::getPercent()
{ return _percent; }
