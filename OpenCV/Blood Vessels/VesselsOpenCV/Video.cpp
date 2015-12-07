#include "Video.h"
#include "iostream"

Video::Video(const char* name)
{
	_name = name;
	VideoCapture* capture = new VideoCapture(name);

	if (capture->isOpened())
	{
		_rate = capture->get(CV_CAP_PROP_FPS);
		_lenght = capture->get(CV_CAP_PROP_FRAME_COUNT);
		_frames = new Mat[_lenght];

		Mat frame;

		for (int i = 0; i < _lenght; i++)
		{
			capture->read(frame);
			_frames[i] = frame.clone();
		}

		capture->release();
	}
	else
	{
		_lenght = _rate = 0;
		_frames = nullptr;
		_name = nullptr;
	}
}

Video::Video(Mat* frames, Video* info)
{
	_name = info->GetFullName();

	_lenght = info->GetLenght();
	_frames = frames;

	_rate = info->GetRate();
}

Video::~Video()
{
	if (_frames)
	{
		for (int i = 0; i < _lenght; i++)
			_frames[i].release();

		delete[] _frames;
		_frames = nullptr;
	}

	_lenght = _rate = 0;
}

Mat* Video::GetFrames()
{
	return _frames;
}

Mat* Video::GetFrameSequence(int length)
{
	if (_lenght < length)
		return nullptr;

	Mat* sequence = new Mat[length];

	for (int sequenceIndex = 0; sequenceIndex < length; sequenceIndex++)
	{
		int nextFrameIndex = sequenceIndex * floor(_lenght / length) + floor(_lenght / (2 * length));
		sequence[sequenceIndex] = _frames[nextFrameIndex].clone();
	}

	return sequence;
}

Mat Video::GetFrame(int index)
{
	return _frames[index];
}

int Video::GetLenght()
{
	return _lenght;
}

int Video::GetRate()
{
	return _rate;
}

const char* Video::GetFullName()
{
	return _name;
}

char* Video::GetFileName()
{
	if (_name == nullptr)
		return nullptr;

	char* fname = new char[strlen(_name)];
	_splitpath(_name, nullptr, nullptr, fname, nullptr);
	return fname;
}

void Video::ShowFrame(int index)
{
	if (_frames != nullptr)
	{
		char* windowName = GetFileName();
		namedWindow(windowName);
		imshow(windowName, _frames[index]);
		char key = waitKey();
		destroyWindow(windowName);
	}
}

void Video::ShowFrames(int rate)
{
	// Ноль для значения по умолчанию.
	rate = (rate > 0) ? rate : _rate;

	if (_frames != nullptr)
	{
		char* windowName = GetFileName();

		namedWindow(windowName);

		for (int i = 0; i < _lenght; i++)
		{
			imshow(windowName, _frames[i]);

			char key = waitKey(1000 / rate);

			// Нажатие Esc.
			if (key == 27)
				break;
		}

		destroyWindow(windowName);
	}
}

void Video::SaveFrame(int index, char* folder)
{
	if (_frames != nullptr)
	{
		string folderName = CreateFolder(folder);

		imwrite(CreateImageName(index, folderName), _frames[index]);
	}
}

void Video::SaveFrames(char* folder)
{
	if (_frames != nullptr)
	{
		string folderName = CreateFolder(folder);

		for (int i = 0; i < _lenght; i++)
			imwrite(CreateImageName(i, folderName), _frames[i]);
	}
}

string Video::CreateFolder(char* folder)
{
	string folderName = folder ? folder : GetFileName();
	_mkdir(folderName.c_str());
	return folderName;
}

string Video::CreateImageName(int index, string path)
{
	return string(path + '\\' + to_string(index) + IMAGE_EXTENSION);
}