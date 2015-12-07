#pragma once

#include "Filter.h"
#include "GaussianKernel.h"

using namespace cv;
using namespace std;

#define DEFAULT_SEGMENT_LENGTH 15
#define DEFAULT_NUMBER_OF_ANGLES 12

class GaussianFilter : public Filter
{
	int _numberOfAngles;
	int _numberOfSigmas;

	GaussianKernel*** _kernels;
	Mat* _intermediateResults;

public:
	GaussianFilter(int sigmaStart = DEFAULT_SIGMA_START, int sigmaEnd = DEFAULT_SIGMA_END,
		int l = DEFAULT_SEGMENT_LENGTH, int numberOfAngles = DEFAULT_NUMBER_OF_ANGLES);
	~GaussianFilter();

	Mat Execute(Mat source);
};

