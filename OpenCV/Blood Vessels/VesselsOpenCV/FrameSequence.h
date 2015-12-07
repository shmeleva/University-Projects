#pragma once

#include <opencv2\opencv.hpp>

#include "Video.h"
#include "Filter.h"

#define DEFAULT_SEQUENCE_LENGTH 3
#define DEFAULT_FRAME_SIZE Size(512, 512)

typedef unsigned char byte;

using namespace cv;

class FrameSequence
{
	const char* _videoPath;
	int   _sequenceLength;
	Mat*  _frameSequence;
	Size  _frameSize;

	FrameSequence(const char* videoPath, Mat* frameSequence, int sequenceLength = DEFAULT_SEQUENCE_LENGTH, Size frameSize = DEFAULT_FRAME_SIZE);

public:
	FrameSequence(const char* videoPath, Filter* filter = nullptr, int sequenceLength = DEFAULT_SEQUENCE_LENGTH, Size frameSize = DEFAULT_FRAME_SIZE);
	~FrameSequence();

	// Возвращает копию последовательности кадров в формате байтового массива.
	vector<byte> ToByteSequence();
};

