#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

#include "MatTools.h"

using namespace cv;
using namespace std;

#define	CLAMP_int(a)	MAX(MIN(a,255),0)
#define	CLAMP_01(a)	    MAX(MIN(a,1),0)

//---------------------------------------------------------------------------
// drawStraigthLine
//---------------------------------------------------------------------------
void drawStraigthLine(const Mat& original, Mat& image, Point pt1, Point pt2, float decrement)
{
	int rows = image.rows;
	int cols = image.cols;

	int m_strokeThickness = 1 ;

	LineIterator lineIterator(image, pt1, pt2, 8, true);
	
	int i;
	int count = lineIterator.count;
	static Mat visited(rows,cols,CV_8U);
	visited.setTo(0);

	for(i = 0; i < count; i++, ++lineIterator)
	{
		int thickness = m_strokeThickness - 1;
		for(int x = -thickness; x <= thickness; x++)
		{
			for(int  y= -thickness; y <= thickness; y++)
			{
				Point pos = lineIterator.pos() + Point(x,y);
				if(pos.x > 0 && pos.x < cols && pos.y > 0 && pos.y < rows)
				{
					if(visited.at<unsigned char>(pos) == 0)
					{
						Vec3f& pix = image.at<Vec3f>(pos);		
						const Vec3f& o =  original.at<Vec3f>(pos);
						if(decrement < 0)
							pix += (Vec3f(1.0f,1.0f,1.0f) - o) * decrement;
						else
							pix += o * decrement;

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

//---------------------------------------------------------------------------
Mat strokebasedGradient(const Mat& imageSrc, const Mat& gradX, const Mat& gradY, const int m_nbStroke)
{
	float m_threshold = 1 ;
	float m_decrement = -0.2f ;

	// Calculer le module du gradient
	Mat gradientAmplitude, gradientOrientation;
	magnitude(gradX, gradY, gradientAmplitude);	

	namedWindow( "GradientMagnitude", WINDOW_AUTOSIZE );
	imshow( "GradientMagnitude", norm_0_255(gradientAmplitude) );  

	// L'image de résultat est blanche au départ
	Vec3f m_startColor(1.0f,1.0f,1.0f);
	Mat dest(imageSrc.rows,imageSrc.cols,imageSrc.type());
	dest.setTo(m_startColor);

	namedWindow( "IntermediateResults", WINDOW_AUTOSIZE );// Create a window for display.
	imshow( "IntermediateResults", dest );  // Show our image.
	waitKey(100);

	// Initialisation pour le tirage aléatoire
	RNG rng(0xFFFFFFFF);

	// Boucle pour tracer les traits
	for(unsigned int i=0;i<m_nbStroke;i++)
	{		
		// Tirage d'un point p sur le domaine de définition de l'image
		Point p;
		p.x = rng.uniform(0, gradX.cols);
		p.y = rng.uniform(0, gradX.rows);

		// On souhaite tracer le trait dans le sens du contours 
		// Remarque: on peut aussi essayer de tracer les traits dans le sens du gradient		
		//Vec2d g(xxxx, xxxx);
		
		// Si le gradient du point tiré aléatoirement est supérieur à un seuil, on trace un trait... sinon on ne fait rien
		/*if ( ..... )
		{	
			// Plusierus solutions: il est préférable de normaliser le gradient en calculant sa norme ||V||=sqrt(a*a+b*b), utiliser la fonction norm
			// norm(g)
			double length = norm(g) ;
			g = (length / 2) * (g / norm(g)); // coeff. 2 empirique

			Point2i p1((int)(p.x-g[0]), (int)(p.y-g[1]));
			Point2i p2((int)(p.x+g[0]), (int)(p.y+g[1]));
			
			drawStraigthLine(imageSrc,dest,p1,p2,m_decrement);
			
			//imshow( "IntermediateResults", dest );
		    //waitKey(100);
		}*/	
		
	}
	return dest;
}
