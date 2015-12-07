#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;

#define DEFAULT_SIGMA_START 1
#define DEFAULT_SIGMA_END 3

class Filter
{
public:
	virtual Mat Execute(Mat source) = 0;
};

