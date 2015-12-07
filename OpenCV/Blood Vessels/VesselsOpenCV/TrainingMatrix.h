#pragma once
#include <opencv2\opencv.hpp>
#include <fstream>

#include "FrameSequence.h"
#include "GaussianFilter.h"
#include "Class.h"

typedef unsigned char byte;

using namespace std;
using namespace cv;

class TrainingMatrix
{
	Filter* _filter;
	int _sequenceLength;
	Size _frameSize;

	vector<byte> _trainingMatrixVector;
	vector<byte> _labelsVector;

	int _trainingImageLength;
	int _numberOfTrainingImages;

	Mat _trainingMatrix;
	Mat _labels;

	// «агрузка тренировочных изображений. 
	// ¬озвращает количество загруженных изображений.
	int Load(char* path, int label);

public:
	TrainingMatrix(char* norm, char* pathalogy, Filter* filter = nullptr, int sequenceLength = DEFAULT_SEQUENCE_LENGTH, Size frameSize = DEFAULT_FRAME_SIZE);
	~TrainingMatrix();

	Mat GetTrainingMatrix();
	Mat GetLabels();
};

