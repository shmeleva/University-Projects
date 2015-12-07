#pragma once

#include <opencv2\opencv.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>

using namespace cv;
using namespace std;

#define IMAGE_EXTENSION ".jpg"
#define DEFAULT_RATE 24

// ’ранит кадры и данные о загруженном видео.
class Video
{
	const char* _name;
	int _rate;
	int _lenght;
	Mat* _frames;

	Video(Mat* frames, Video* info);

	string CreateFolder(char* folder = nullptr);
	string CreateImageName(int index, string path);

public:

	Video(const char* name);
	~Video();

	// —оздает и возвращает массив равноудаленных кадров размерности length.
	// ¬озврашаемый массив содержит копии кадров.
	// ≈сли количество кадров в видео меньше параметра length, возвращает nullptr.
	Mat* GetFrameSequence(int length);

	// Accessors:

	// ѕолучает массив кадров.
	Mat* GetFrames();

	// ѕолучает кадр с индексом index.
	Mat GetFrame(int index);

	// ѕолучает количество кадров в видео.
	int GetLenght();

	// ѕолучает кадровую частоту в кадрах в секунду.
	int GetRate();

	// ѕолучает путь к исходному видео.
	const char* GetFullName();

	// ѕолучает им€ исходного видео.
	char* GetFileName();

	// Output:

	void ShowFrame(int index);
	void ShowFrames(int rate = 0);

	void SaveFrame(int index, char* folder = nullptr);
	void SaveFrames(char* folder = nullptr);
};

