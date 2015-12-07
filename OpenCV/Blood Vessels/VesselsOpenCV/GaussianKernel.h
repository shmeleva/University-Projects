#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class GaussianKernel
{
	const int _sigmaMultiplier = 3;

	int _zero;
	Mat _matKernel;

public:
	GaussianKernel(int sigma, int l);
	GaussianKernel(Mat kernel);
	~GaussianKernel();

	Mat GetKernel();
	GaussianKernel* CvRotate(float angle);
	Mat Filter(Mat source);
};

