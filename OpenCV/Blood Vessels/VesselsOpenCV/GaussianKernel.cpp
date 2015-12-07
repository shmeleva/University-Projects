#include "GaussianKernel.h"

#define PI 3.14159

GaussianKernel::GaussianKernel(int sigma, int l)
{
	// Размеры значимой части маски.
	int width = sigma * _sigmaMultiplier * 2 + 1;
	int height = (l / 2) * 2 + 1;

	// Реальный размер маски, определающийся как диаметр окружности,
	// описанной вокруг значимой части маски.
	int diameter = round(sqrt(pow((width / 2), 2) + pow((height / 2), 2))) * 2 + 1;

	// Центр координат по оси абсцисс (и ординат).
	_zero = diameter / 2;

	// Объявление и инициализация нулевыми значениями квадратной маски
	// размера _diameter на _diameter.
	float* kernel = new float[diameter * diameter];
	for (int x = 0; x < diameter * diameter; x++)
		kernel[x] = 0.0;

	float sum = 0;

	// Вертикальный и горизонтальный отступы значимой части от краев маски.
	int verticalOffset = (diameter - height) / 2,
		horizontalOffset = (diameter - width) / 2;

	// Заполнение фильтра значениями и подсчет суммы элементов.
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			sum += kernel[(y + verticalOffset) * diameter + (x + horizontalOffset)] = -(1.0 / ((float)sigma * sqrt(2 * PI))) * exp(-pow((x + horizontalOffset) - _zero, 2) / (2 * pow(sigma, 2)));

	// Mean value.
	float m = sum / (width * height);

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			kernel[(y + verticalOffset) * diameter + (x + horizontalOffset)] -= m;

	_matKernel = Mat(diameter, diameter, CV_32F, kernel);
}

GaussianKernel::GaussianKernel(Mat kernel)
{
	_matKernel = kernel;
	_zero = kernel.rows / 2;
}

GaussianKernel::~GaussianKernel()
{
	_matKernel.release();
	_zero = 0;
}

Mat GaussianKernel::GetKernel()
{
	return _matKernel;
}

GaussianKernel* GaussianKernel::CvRotate(float angle)
{
	if (angle == 0)
		return this;

	Point2f center(_zero, _zero);
	Mat rotationMatrix = getRotationMatrix2D(center, angle, 1.0);
	Mat rotated = Mat::zeros(_matKernel.rows, _matKernel.cols, _matKernel.type());

	warpAffine(_matKernel, rotated, rotationMatrix, _matKernel.size());

	return new GaussianKernel(rotated);
}

Mat GaussianKernel::Filter(Mat source)
{
	Mat destination = Mat::zeros(source.rows, source.cols, source.type());
	cv::filter2D(source, destination, -1, _matKernel);
	return destination;
}

