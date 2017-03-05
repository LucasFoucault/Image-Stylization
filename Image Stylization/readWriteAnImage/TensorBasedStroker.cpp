#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O

using namespace cv;
using namespace std;

#include "TensorBasedStroker.h"
#include "MatTools.h"

/*
 * Calcul des normes du gradient de l'image en paramètre et affichage de celle-ci,
 * puis calcul de l'orientation des gradients
 */
void TensorBasedStroker::PreProcess(const Mat& original, const float& percent)
{
	// Taille du noyau gaussien
	int kernel = (int)original.rows / 50.0f;

	// Application d'un flou gaussien pour débruiter l'image
	Mat blurredImage = MatTools::gaussianRegularization(original, kernel);

	// Décomposition de l'image filtrée en trois composantes L, a et b
	Mat labImage;
	cvtColor(blurredImage, labImage, CV_BGR2Lab);
	
	// Séparation de chaque composante de l'imgae L, a et b
	Mat splitLabImage[3];
	split(labImage, splitLabImage);

	// Calcul des gradients avec Sobel
    Mat gradX, gradY;
	Sobel(splitLabImage[0], gradX, CV_32F, 1, 0);
	Sobel(splitLabImage[0], gradY, CV_32F, 0, 1);

	// Création de matrices de même dimension que l'image en paramètre 
	_eigenValues = Mat(original.rows, original.cols, CV_32FC2);
	_edgeNormals = Mat(original.rows, original.cols, CV_32FC2);
	_isophotes	 = Mat(original.rows, original.cols, CV_32FC2);
	_normValue	 = Mat(original.rows, original.cols, CV_32FC1);
	
	// Calcul des tenseurs de structures
	Mat g11Mat = gradX.mul(gradX);
	Mat g12Mat = gradX.mul(gradY);
	Mat g22Mat = gradY.mul(gradY);

	// Régularisation
	g11Mat = MatTools::gaussianRegularization(g11Mat, kernel);
	g12Mat = MatTools::gaussianRegularization(g12Mat, kernel);
	g22Mat = MatTools::gaussianRegularization(g22Mat, kernel);

	// Récupération des tenseurs de l'image
	for (int i = 0; i < original.rows; i++)
		for (int j = 0; j < original.cols; j++)
		{
			// Calcul des tenseurs de structures
			float g11 = g11Mat.at<float>(i, j);
			float g12 = g12Mat.at<float>(i, j);
			float g22 = g22Mat.at<float>(i, j);

			// Calcul du Delta afin d'obtenir les valeurs et vecteurs propres
			float delta = (g11 - g22) * (g11 - g22) + 4 * g12 * g12;

			// Calcul des valeurs propres
			float eigenValueMinus = (g11 + g22 - sqrt(delta)) / 2;
			float eigenValuePlus  = (g11 + g22 + sqrt(delta)) / 2;

			// Calcul des vecteurs propres
			Vec2f eigenVectorMinus = Vec2f(2 * g12, g22 - g11 - sqrt(delta));
			Vec2f eigenVectorPlus  = Vec2f(2 * g12, g22 - g11 + sqrt(delta));

			// Enregistre le tout dans des matrices
			_eigenValues.at<Vec2f>(i, j) = Vec2f(eigenValueMinus, eigenValuePlus);
			_edgeNormals.at<Vec2f>(i, j) = eigenVectorPlus;
			_isophotes.at<Vec2f>(i, j)	 = eigenVectorMinus;

			// Calcul les normes des valeurs propres

			// Utilisation de la norme Le Meur (à vos risques et périls)
			//_normValue.at<float>(i, j) = pow((eigenValuePlus - eigenValueMinus) / (eigenValuePlus + eigenValueMinus), 2);
				
			// Utilisation de la norme Goudé
			_normValue.at<float>(i, j) = pow((1.0f + eigenValuePlus + eigenValueMinus), 0.5f);
		}

	_normValue = MatTools::norm_0_1(_normValue);

	DefineTreshold(percent);
	ListValidPoints(percent);

	// Carte des normes de tenseurs
	//imshow("Tensor Map", MatTools::norm_0_255(_normValue));
	//waitKey(10);
}

/*
 * 
 */
void TensorBasedStroker::DefineTreshold(const float& percent)
{
	// Establish the number of bins
	int histSize = 1000;

	// Set the ranges ( for B,G,R) )
	float range[] = { 0.0f, 1.0f } ;
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat norm_hist;

	// Compute the histograms
	calcHist(&_normValue, 1, 0, Mat(), norm_hist, 1, &histSize, &histRange, uniform, accumulate);

	int accumulation = 0;
	for(int i = norm_hist.rows-1; i>=0; i--)
	{
		accumulation += norm_hist.at<float>(i);

		if(accumulation > percent*_normValue.rows*_normValue.cols)
		{
			cout << "Accumulation = " << accumulation << endl;
			cout << "Result = " << i / (float)histSize << endl;
			
			_threshold = i / (float)histSize; 
			break;
		}
	}
}


/*
 * 
 */
void TensorBasedStroker::ListValidPoints(const float& percent)
{
	for (int i = 0; i < _normValue.rows; i++)
		for (int j = 0; j < _normValue.cols; j++)
		{
			Point point(j, i);

			if(_normValue.at<float>(point) > _threshold || percent == 1.0f)
			{
				_validPoints.push_back(point);
				_validEigenValues.push_back(_eigenValues.at<Vec2f>(i, j));
				_validEdgeNormals.push_back(_edgeNormals.at<Vec2f>(i, j));
				_validIsophotes.push_back(_isophotes.at<Vec2f>(i, j));
			}
			else
			{
				_normValue.at<float>(point) = 0;
			}
		}

	string nameFile = "Results/TensorMap_" + to_string(percent) + "percent.png";
	imwrite(nameFile, MatTools::norm_0_255(_normValue));
	imshow("Tensor Map", MatTools::norm_0_255(_normValue));
	waitKey(10);
}