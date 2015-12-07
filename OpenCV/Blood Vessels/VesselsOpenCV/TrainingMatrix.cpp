#include "TrainingMatrix.h"
#include "iostream"

using namespace std;

TrainingMatrix::TrainingMatrix(char* norm, char* pathalogy, Filter* filter, int sequenceLength, Size frameSize)
{
	_filter = filter;

	_sequenceLength = sequenceLength;
	_frameSize = frameSize;
	_trainingImageLength = _sequenceLength * _frameSize.area();

	_numberOfTrainingImages = 0;
	_numberOfTrainingImages += Load(norm, (int)Class::Norm);
	_numberOfTrainingImages += Load(pathalogy, (int)Class::Pathalogy);

	_trainingMatrix = Mat(_numberOfTrainingImages, _trainingImageLength, CV_8UC1, _trainingMatrixVector.data());
	_labels = Mat(_numberOfTrainingImages, 1, CV_8UC1, _labelsVector.data());

	//imwrite("training_filtered.jpg", Mat(frameSize.height  * _sequenceLength * _numberOfTrainingImages, frameSize.width, CV_8UC1, _trainingMatrixVector.data()));
}


TrainingMatrix::~TrainingMatrix()
{
	_trainingMatrix.release();
	_labels.release();
}

int TrainingMatrix::Load(char* path, int label)
{
	ifstream* videoList = new ifstream(path);
	string nextVideo;
	int count = 0;

	while (getline(*videoList, nextVideo))
	{
		FrameSequence* filtered = new FrameSequence(nextVideo.c_str(), _filter, _sequenceLength, _frameSize);
		vector<byte> nextTrainingImage = filtered->ToByteSequence();

		if (nextTrainingImage.size() == _trainingImageLength)
		{
			_trainingMatrixVector.insert(_trainingMatrixVector.end(), nextTrainingImage.begin(), nextTrainingImage.end());
			_labelsVector.push_back(label);
			count++;
		}

		delete filtered;
	}

	videoList->close();

	return count;
}

Mat TrainingMatrix::GetTrainingMatrix()
{
	Mat floatTrainingMatrix;
	_trainingMatrix.convertTo(floatTrainingMatrix, CV_32F);

	return floatTrainingMatrix;
}

Mat TrainingMatrix::GetLabels()
{
	Mat floatLabels;
	_labels.convertTo(floatLabels, CV_32SC1);

	return floatLabels;
}