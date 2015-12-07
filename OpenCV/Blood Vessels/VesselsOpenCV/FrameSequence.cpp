#include "FrameSequence.h"

FrameSequence::FrameSequence(const char* videoPath, Filter* filter, int sequenceLength, Size frameSize)
{
	// Сохранение кадров из видео.

	_videoPath = videoPath;
	_sequenceLength = sequenceLength;
	_frameSize = frameSize;

	Video* video = new Video(_videoPath);
	_frameSequence = video->GetFrameSequence(_sequenceLength);

	if (video)
	{
		delete video;
		video = nullptr;
	}

	if (_frameSequence == nullptr)
		_sequenceLength = 0;

	// Изменение размера кадров.

	for (int i = 0; i < _sequenceLength; i++)
		cv::resize(_frameSequence[i], _frameSequence[i], _frameSize);

	// Применения фильтра, если задан.

	if (filter)
	{
		for (int i = 0; i < _sequenceLength; i++)
		{
			Mat temp = _frameSequence[i];
			_frameSequence[i] = filter->Execute(_frameSequence[i]);
			temp.release();
		}
	}
}

FrameSequence::FrameSequence(const char* videoPath, Mat* frameSequence, int sequenceLength, Size frameSize)
{
	_videoPath = videoPath;
	_frameSequence = frameSequence;
	_sequenceLength = sequenceLength;
	_frameSize = frameSize;
}

FrameSequence::~FrameSequence()
{
	if (_frameSequence)
	{
		for (int i = 0; i < _sequenceLength; i++)
			_frameSequence[i].release();

		delete[] _frameSequence;
		_frameSequence = nullptr;
	}

	_sequenceLength = 0;
}

vector<byte> FrameSequence::ToByteSequence()
{
	int frameArea = _frameSize.width * _frameSize.height;
	int byteSequenceLength = _sequenceLength * frameArea;
	
	vector<byte> data = vector<byte>(byteSequenceLength);

	for (int frameIndex = 0; frameIndex < _sequenceLength; frameIndex++)
	{
		if (_frameSequence[frameIndex].channels() > 1)
			cv::cvtColor(_frameSequence[frameIndex], _frameSequence[frameIndex], COLOR_BGR2GRAY);

		for (int pixelIndex = 0; pixelIndex < frameArea; pixelIndex++)
			data[frameIndex * frameArea + pixelIndex] = _frameSequence[frameIndex].data[pixelIndex];
	}

	return data;
}

