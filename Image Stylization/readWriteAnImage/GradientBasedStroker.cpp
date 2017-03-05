#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;
using namespace std;

#include "GradientBasedStroker.h"
#include "MatTools.h"

/*
 * Calcul des normes du gradient de l'image en paramètre et affichage de celle-ci,
 * puis calcul de l'orientation des gradients
 *
 */
void GradientBasedStroker::PreProcess(const Mat& original, const float& percent)
{
	// Décomposition de l'image filtrée en trois composantes L, a et b
	Mat labImage;
	cvtColor(original, labImage, CV_BGR2Lab);
	
	// Séparation de chaque composante de l'imgae L, a et b
	Mat splitLabImage[3];
	split(labImage, splitLabImage);

	// Calcul des gradients avec Sobel
    Mat gradX, gradY;
	Sobel(splitLabImage[0], gradX, CV_32F, 1, 0);
	Sobel(splitLabImage[0], gradY, CV_32F, 0, 1);

	// Calculer le module du gradient
	magnitude(gradX, gradY, _gradientAmplitude);

	// Affichage de la norme des gradients de l'image
	namedWindow("GradientMagnitude", WINDOW_AUTOSIZE);
	imshow("GradientMagnitude", MatTools::norm_0_255(_gradientAmplitude));

	// Création d'une matrice de même dimension que l'image en paramètre 
	_gradientOrientation = Mat(original.rows, original.cols, CV_32FC2);

	// Récupération de l'orientation des gradients 
	for (int i = 0; i < _gradientOrientation.rows; i++)
		for (int j = 0; j < _gradientOrientation.cols; j++)
			_gradientOrientation.at<Vec2f>(i, j) = Vec2f(gradX.at<float>(i, j), gradY.at<float>(i, j));

	_gradientAmplitude = MatTools::norm_0_1(_gradientAmplitude);

	DefineTreshold(percent);
	ListValidPoints(percent);
}


/*
 * 
 */
void GradientBasedStroker::DefineTreshold(const float& percent)
{
	/// Establish the number of bins
	int histSize = 100;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0.0f, 1.0f } ;
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat norm_hist;

	/// Compute the histograms:
	calcHist(&_gradientAmplitude, 1, 0, Mat(), norm_hist, 1, &histSize, &histRange, uniform, accumulate);

	int accumulation = 0;
	for(int i = norm_hist.rows-1; i>=0; i--)
	{
		accumulation += norm_hist.at<float>(i); // PROBLEME LA !!!
		cout << norm_hist.at<float>(i) << endl;

		if(accumulation > percent*_gradientAmplitude.rows*_gradientAmplitude.cols)
		{
			cout << "accumulation = " << accumulation << endl;
			cout << "result = " << i / (float)histSize << endl;
			
			_threshold = i / (float)histSize; 
			break;
		}
	}
}


/*
 * 
 */
void GradientBasedStroker::ListValidPoints(const float& percent)
{
	for (int i = 0; i < _gradientAmplitude.rows; i++)
		for (int j = 0; j < _gradientAmplitude.cols; j++)
		{
			Point point(j, i);

			if(_gradientAmplitude.at<float>(point) > _threshold)
				_validPoints.push_back(point);
		}	
}