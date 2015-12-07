#include <iostream>

#include "Video.h"
#include "GaussianFilter.h"
#include "FrangiFilter.h"
#include "FrameSequence.h"
#include "TrainingMatrix.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

#define TEST "se001.avi"

int main(int argc, char* argv[])
{
	// Training:
	GaussianFilter* filter = new GaussianFilter();
	TrainingMatrix* training = new TrainingMatrix("norm.txt", "pathalogy.txt", filter);

	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::POLY);
	svm->setGamma(3);
	svm->setDegree(2);

	Mat trainingMatrix = training->GetTrainingMatrix();
	Mat labels = training->GetLabels();
	Ptr<TrainData> trainingData = TrainData::create(trainingMatrix, SampleTypes::ROW_SAMPLE, labels);
	svm->train(trainingData);

	FrameSequence* temp = new FrameSequence(TEST, filter);
	vector<byte> data = temp->ToByteSequence();
	Mat input = Mat(1, data.size(), CV_8UC1, data.data());
	Mat floatInput;
	input.convertTo(floatInput, CV_32F);

	// Response:
	Mat response;
	svm->predict(floatInput, response);
	cout << response;

	// Training data.
	svm->save("database.xml");

	return 0;
}