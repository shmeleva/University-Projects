#pragma once

#include <opencv2\opencv.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>

using namespace cv;
using namespace std;

#define IMAGE_EXTENSION ".jpg"
#define DEFAULT_RATE 24

// ������ ����� � ������ � ����������� �����.
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

	// ������� � ���������� ������ �������������� ������ ����������� length.
	// ������������ ������ �������� ����� ������.
	// ���� ���������� ������ � ����� ������ ��������� length, ���������� nullptr.
	Mat* GetFrameSequence(int length);

	// Accessors:

	// �������� ������ ������.
	Mat* GetFrames();

	// �������� ���� � �������� index.
	Mat GetFrame(int index);

	// �������� ���������� ������ � �����.
	int GetLenght();

	// �������� �������� ������� � ������ � �������.
	int GetRate();

	// �������� ���� � ��������� �����.
	const char* GetFullName();

	// �������� ��� ��������� �����.
	char* GetFileName();

	// Output:

	void ShowFrame(int index);
	void ShowFrames(int rate = 0);

	void SaveFrame(int index, char* folder = nullptr);
	void SaveFrames(char* folder = nullptr);
};

