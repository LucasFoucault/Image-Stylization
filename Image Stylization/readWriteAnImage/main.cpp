//
//  main file
//  Stroke-based stylisation
//	LowPoly-based stylisation
//

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <iostream> // for standard I/O
#include <cstdlib>
#include <ctime>

using namespace cv;
using namespace std;

#include "StrokeRenderer.h"
#include "StraightStroker.h"
#include "TensorStraightStroker.h"
#include "TensorCurvedStroker.h"
#include "PyramidalStrokeRenderer.h"
#include "TensorBrushStroker.h"

#include "DelaunayTriangulation.h"
#include "VoronoiDiagram.h"
#include "LowPolyRenderer.h"

#include "Particle.h"

#include "MatTools.h"

void strokeStylization(const Mat& image, Mat& result, const int nbStroke, float percent, bool blackAndWhite, int choix)
{
	if (choix == 1)
	{
		StraightStroker stroker(1, -0.2f, blackAndWhite);
		stroker.PreProcess(image, percent);

		StrokeRenderer renderer(percent, stroker);
		renderer.Process(image, result, &nbStroke);
	} 
	else if (choix == 2)
	{
		TensorStraightStroker stroker(1, -0.2f, blackAndWhite);
		stroker.PreProcess(image, percent);

		StrokeRenderer renderer(percent, stroker);
		renderer.Process(image, result, &nbStroke);
	}
	else if (choix == 3)
	{
		TensorCurvedStroker stroker(1, -0.2f, blackAndWhite);
		stroker.PreProcess(image, percent);

		StrokeRenderer renderer(percent, stroker);
		renderer.Process(image, result, &nbStroke);
	}
	else if (choix == 4)
	{
		string nameImage;

		cout << endl;
		cout << "Veuillez entrer le nom de l'une des brosses suivantes afin de la selectionner :" << endl;
		cout << "> brush1" << endl;
		cout << "> brush2" << endl;
		cout << "> brush3" << endl;
		cout << "> brush4" << endl;
		cout << "> custom1" << endl;
		cout << "> pencil1" << endl;
		cout << "> pencil2" << endl;
		cout << "> pencil3" << endl;
		cin >> nameImage;

		Mat3f brush = imread("../brush/" + nameImage + ".png");

		TensorBrushStroker stroker(brush);
		stroker.PreProcess(image, percent);

		StrokeRenderer renderer(percent, stroker);
		renderer.Process(image, result, &nbStroke);
	}
}

void strokePyramidalStylization(const Mat& image, Mat& result, const int nbStroke, float percent, bool blackAndWhite, int choix)
{
	const int levels = 3;

	vector<IStroker*> strokers;
	vector<StrokeRenderer*> renderers;

	if (choix == 1)
	{
		// Higher resolution
		strokers.push_back(new TensorStraightStroker(1, -0.2f, blackAndWhite));
		strokers.push_back(new TensorStraightStroker(1, -0.2f, blackAndWhite));
		strokers.push_back(new TensorStraightStroker(1, -0.2f, blackAndWhite));
		// Lower resolution
	}
	else if (choix == 2)
	{
		// Higher resolution
		strokers.push_back(new TensorCurvedStroker(1, -0.2f, blackAndWhite));
		strokers.push_back(new TensorCurvedStroker(1, -0.2f, blackAndWhite));
		strokers.push_back(new TensorCurvedStroker(1, -0.2f, blackAndWhite));
		// Lower resolution
	}
	else
		return;

	renderers.push_back(new StrokeRenderer(percent, *strokers[0]));
	renderers.push_back(new StrokeRenderer(percent, *strokers[1]));
	renderers.push_back(new StrokeRenderer(percent, *strokers[2]));

	PyramidalStrokeRenderer renderer(image, levels, strokers, renderers);
	int nbStrokeByLevel[levels];
	nbStrokeByLevel[0] = nbStroke;
	nbStrokeByLevel[1] = nbStroke/2;
	nbStrokeByLevel[2] = nbStroke/4;
	renderer.Process(image, result, nbStrokeByLevel);
}

void lowPolyStylization(const Mat& image, Mat& result, const int nbPoints, float percent, bool blackAndWhite, VideoWriter video, int choix)
{
	TensorStraightStroker stroker(1, -0.2f, blackAndWhite);

	if (choix == 1)
	{
		DelaunayTriangulation lowPoly;
		lowPoly.PreProcess(image, stroker, percent, &nbPoints);

		LowPolyRenderer renderer(lowPoly);
		renderer.Process(image, result, choix, video);
	}
	else if (choix == 2)
	{
		VoronoiDiagram lowPoly;
		lowPoly.PreProcess(image, stroker, percent, &nbPoints);

		LowPolyRenderer renderer(lowPoly);
		renderer.Process(image, result, choix, video);
	}
}

void flowFieldStylization(const Mat& image, Mat& result, float percent, int nbParticle, float maxSpeed, int interval, int nbLoop, bool blackAndWhite, int choix)
{
	if (choix == 1)
		percent = 1.0f;

	TensorStraightStroker stroker(1, -0.2f, blackAndWhite);
	stroker.PreProcess(image, percent);

	vector<Particle> particles;
	srand(time(0));  // needed once per program run

	for (int i = 0; i < nbParticle; i++)
	{
		Particle *newParticle = new Particle(image.cols, image.rows, maxSpeed);
		particles.push_back(*newParticle);
	}

	int cols = floor(result.cols / interval);
	int rows = floor(result.rows / interval);
	vector<Vec2f> isophotes;

	particles[0].setUp(isophotes, stroker, interval, rows, cols, choix);


	//imshow("IntermediateResults", MatTools::norm_0_255(result));
	//int keyPressed = -1;
	//while (keyPressed == -1)
	//{
	//	keyPressed = waitKey(1);
	//	if (keyPressed != -1)
	//		cout << "KEY pressed" << endl;
	//}

	for (int i = 0; i < nbLoop; i++)
	{
		for (int j = 0; j < nbParticle; j++)
		{
			particles[j].follow(stroker._validPoints, isophotes, interval, cols, choix);
			particles[j].update();
			particles[j].edges(result.cols, result.rows);
			particles[j].show(image, result);
		}

		imshow("IntermediateResults", MatTools::norm_0_255(result));
		waitKey(10);
	}
}

void writeAndSave(Mat& result, Scalar color, string nameResult)
{
	imwrite(nameResult, MatTools::norm_0_255(result));
	cout << "Image sauvegardee sous le nom " + nameResult << endl;

	// Close all windows
	destroyAllWindows();

	// Display final image
	namedWindow("FinalResult", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("FinalResult", MatTools::norm_0_255(result)); // Show our image.
	waitKey(10);

	result.setTo(color);
}

int main(int argc, const char * argv[])
{
	cout << "CAV : STYLISATION D'IMAGE" << endl;
	cout << "-------------------------" << endl;

	// Load image
	Mat image = imread("../caca.jpg");

	string nameResult;
	Vec3f color(1.0f, 1.0f, 1.0f);
	Mat result(image.rows, image.cols, CV_32FC3);
	result.setTo(color);

	// Paramètre à changer
		bool blackAndWhite = false;
		float percent = 0.20;

		// Stylisation basée strokes et low poly
		const int nbPoints = 3000;

		// Stylisation basee flux des fluides
		const int nbParticle = 500;
		const float maxSpeed = 2.0f;
		const int nbLoop = 500;
		const int interval = 5;

	// Créer outil de capture vidéo
	VideoWriter video;
	Size s = Size(image.rows, image.cols);
	//video.open("LowPoly.avi", -1, 10.0, s, true);

	//if (!video.isOpened())
	//{
	//	cout << "Could not open the output video" << endl;
	//	return -1;
	//}

	int nbResult = 0;
	int choix = 0;
	while (choix < 5)
	{
		cout << endl;
		cout << "1. Stylisation basee strokes" << endl;
		cout << "2. Stylisation basee strokes pyramidals" << endl;
		cout << "3. Stylisation basee low poly" << endl;
		cout << "4. Stylisation basee flux des fluides" << endl;
		cout << "5. Quitter" << endl;
		cin >> choix;
		cout << "-------------------------------------" << endl;

		switch (choix)
		{
		case 1:
			cout << "1. Strokes base gradient" << endl;
			cout << "2. Strokes base tenseurs droits" << endl;
			cout << "3. Strokes base tenseurs courbes" << endl;
			cout << "4. Strokes base tenseurs brosses" << endl;
			cin >> choix;

			strokeStylization(image, result, nbPoints, percent, blackAndWhite, choix);

			nameResult = "Results/Strokes/StrokeStylization_" + to_string(nbResult++) + ".png";
			writeAndSave(result, color, nameResult);
			break;

		case 2:
			cout << "1. Strokes pyramidals base tenseurs droits" << endl;
			cout << "2. Strokes pyramidals base tenseurs courbes" << endl;
			cin >> choix;

			strokePyramidalStylization(image, result, nbPoints, percent, blackAndWhite, choix);

			nameResult = "Results/Pyramidals/StrokePyramidalStylization_" + to_string(nbResult++) + ".png";
			writeAndSave(result, color, nameResult);
			break;

		case 3:
			cout << "1. Low poly base Delaunay" << endl;
			cout << "2. Low poly base Voronoi" << endl;
			cin >> choix;

			lowPolyStylization(image, result, nbPoints, percent, blackAndWhite, video, choix);

			nameResult = "Results/LowPoly/LowPolyStylization_" + to_string(nbResult++) + ".png";
			writeAndSave(result, color, nameResult);
			break;

		case 4:
			cout << "1. Flux de fluide base intervale de tenseurs" << endl;
			cout << "2. Flux de fluide base tenseurs principaux" << endl;
			cin >> choix;

			flowFieldStylization(image, result, percent, nbParticle, maxSpeed, interval, nbLoop, blackAndWhite, choix);

			nameResult = "Results/FlowFluid/FlowFieldStylization_" + to_string(nbResult++) + ".png";
			writeAndSave(result, color, nameResult);
			break;

		default:
			break;
		}
	}

	cout << "Fin de l'application" << endl;
}