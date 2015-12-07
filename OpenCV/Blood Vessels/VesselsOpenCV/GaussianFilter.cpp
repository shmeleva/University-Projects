#include "GaussianFilter.h"

GaussianFilter::GaussianFilter(int sigmaStart, int sigmaEnd, int l, int numberOfAngles)
{
	_numberOfSigmas = sigmaEnd - sigmaStart + 1;
	_numberOfAngles = numberOfAngles;

	float angleOfRotation = 180 / _numberOfAngles;

	_kernels = new GaussianKernel**[_numberOfSigmas];

	for (int sigmaIndex = 0, sigma = sigmaStart; sigmaIndex < _numberOfSigmas; sigmaIndex++, sigma++)
	{
		GaussianKernel* base = new GaussianKernel(sigma, l);

		_kernels[sigmaIndex] = new GaussianKernel*[_numberOfAngles];

		for (int angleIndex = 0; angleIndex < _numberOfAngles; angleIndex++)
			_kernels[sigmaIndex][angleIndex] = base->CvRotate(angleIndex * angleOfRotation);
	}

	_intermediateResults = new Mat[_numberOfSigmas * _numberOfAngles];
}

GaussianFilter::~GaussianFilter()
{
	if (_kernels)
	{
		for (int sigmaIndex = 0; sigmaIndex < _numberOfSigmas; sigmaIndex++)
		{
			if (_kernels[sigmaIndex])
			{
				for (int angleIndex = 0; angleIndex < _numberOfAngles; angleIndex++)
				{
					if (_kernels[sigmaIndex][angleIndex])
						delete _kernels[sigmaIndex][angleIndex];

					_kernels[sigmaIndex][angleIndex] = nullptr;
				}

				delete[] _kernels[sigmaIndex];
				_kernels[sigmaIndex] = nullptr;
			}
		}

		delete[] _kernels;
		_kernels = nullptr;
	}

	if (_intermediateResults)
	{
		delete[] _intermediateResults;
		_intermediateResults = nullptr;
	}

	_numberOfSigmas = _numberOfAngles = 0;
}

Mat GaussianFilter::Execute(Mat source)
{
	cv::cvtColor(source, source, COLOR_BGR2GRAY);
	cv::fastNlMeansDenoising(source, source, 4.0, 7, 9);

	for (int sigmaIndex = 0; sigmaIndex < _numberOfSigmas; sigmaIndex++)
		for (int angleIndex = 0; angleIndex < _numberOfAngles; angleIndex++)
			_intermediateResults[sigmaIndex * _numberOfAngles + angleIndex] = _kernels[sigmaIndex][angleIndex]->Filter(source);

	Mat result = Mat::zeros(source.rows, source.cols, source.type());

	for (int i = 0; i < _numberOfSigmas * _numberOfAngles; i++)
	{
		result = max(result, _intermediateResults[i]);
		//imwrite(to_string(i * (180 / _numberOfAngles)) + ".jpg", _intermediateResults[i]);
	}

	// cv::threshold(result, result, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	return result;
}
