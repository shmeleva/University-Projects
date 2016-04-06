// guro3d.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "guro3d.h"
#include <cmath>
#include <stdio.h>
#include <algorithm>

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

												// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GURO3D, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return false;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GURO3D));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GURO3D));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_GURO3D);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

#define RUSSIAN_KEY_OFFSET 1104

#define MOVE_LEFT_ITEM 8192
#define MOVE_RIGHT_ITEM 8193
#define MOVE_UP_ITEM 8194
#define MOVE_DOWN_ITEM 8195

#define ROTATE_X_ITEM 8196
#define ROTATE_Y_ITEM 8197

#define REFLECT_XY_ITEM 8198
#define REFLECT_X_ITEM 8199
#define REFLECT_Y_ITEM 8200

#define RESIZE_UP_ITEM 8201
#define RESIZE_DOWN_ITEM 8202

#define RESET_ITEM 8203

// Buffer variables:
BITMAPINFO bmi;
DWORD* pixels = nullptr;

#pragma region Custom data types

struct vector3d { double x, y, z; };
typedef struct vector3d VECTOR3D;

struct vertex3d { VECTOR3D point; VECTOR3D normal; };
typedef struct vertex3d VERTEX3D;

struct vector2d { int x, y; };
typedef struct vector2d VECTOR2D;

struct vertex2d { VECTOR2D point; double light; };
typedef struct vertex2d VERTEX2D;

#pragma endregion

#pragma region Template

#define ROOF_POINTS 6
#define ROOF_RECTANGLE_FACES 3
#define ROOF_TRIANGLE_FACES 2
#define ROOF_RECTANGLE_VERTEXES 4
#define ROOF_TRIANGLE_VERTEXES 3

double roofTemplate[ROOF_POINTS][3] = { { -1, -1, 1 },{ 0, -1, -1 },{ 1, -1, 1 }, { -1, 1, 1 },{ 0, 1, -1 },{ 1, 1, 1 } };
double roofTemplateMultiplier = 64;

VERTEX3D roofPoints3D[ROOF_POINTS];
VERTEX2D roofPoints2D[ROOF_POINTS];

int rectangleVerges[ROOF_RECTANGLE_FACES][ROOF_RECTANGLE_VERTEXES] = { { 2, 1, 4, 5 },{ 2,5,3,0 },{ 3, 4, 1, 0 } };
int triangleVerges[ROOF_TRIANGLE_FACES][ROOF_TRIANGLE_VERTEXES] = { { 0, 1, 2 },{ 5, 4, 3 } };

int rectangleColors[ROOF_RECTANGLE_FACES][3] = { { 209, 179, 255 },{ 179, 179, 255 },{ 255,179,255 } };
int triangleColors[ROOF_TRIANGLE_FACES][3] = { { 179,255,255 },{ 255,197,198 } };

VERTEX2D pointList[4096];
int pointListCount = 0;

#pragma endregion

RECT clientRectangle;
VECTOR2D screenCentre;
VECTOR2D rotationPoint;

#pragma region (1) Поворот на заданные углы относительно центра координат и относительно произвольной точки.

// Keys: W A S D

void RotateY(double alpha, VECTOR2D pointOfRotation)
{
	double x, sinA = sin(alpha), cosA = cos(alpha);
	//
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		roofPoints3D[i].point.x -= pointOfRotation.x;
		roofPoints3D[i].point.y -= pointOfRotation.y;
		//
		x = roofPoints3D[i].point.x * cosA - roofPoints3D[i].point.z * sinA;
		roofPoints3D[i].point.z = roofPoints3D[i].point.x * sinA + roofPoints3D[i].point.z * cosA;
		roofPoints3D[i].point.x = x;
		//
		roofPoints3D[i].point.x += pointOfRotation.x;
		roofPoints3D[i].point.y += pointOfRotation.y;
		//
		x = roofPoints3D[i].normal.x * cosA - roofPoints3D[i].normal.z * sinA;
		roofPoints3D[i].normal.z = roofPoints3D[i].normal.x * sinA + roofPoints3D[i].normal.z * cosA;
		roofPoints3D[i].normal.x = x;
	}
}

void RotateX(double alpha, VECTOR2D pointOfRotation)
{
	double y, sinA = sin(alpha), cosA = cos(alpha);
	//
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		roofPoints3D[i].point.x -= pointOfRotation.x;
		roofPoints3D[i].point.y -= pointOfRotation.y;
		//
		y = roofPoints3D[i].point.y * cosA + roofPoints3D[i].point.z * sinA;
		roofPoints3D[i].point.z = roofPoints3D[i].point.z * cosA - roofPoints3D[i].point.y * sinA;
		roofPoints3D[i].point.y = y;
		//
		roofPoints3D[i].point.x += pointOfRotation.x;
		roofPoints3D[i].point.y += pointOfRotation.y;
		//
		y = roofPoints3D[i].normal.y * cosA + roofPoints3D[i].normal.z * sinA;
		roofPoints3D[i].normal.z = roofPoints3D[i].normal.z * cosA - roofPoints3D[i].normal.y * sinA;
		roofPoints3D[i].normal.y = y;
	}
}

#pragma endregion

#pragma region (2) Перенос по оси OX и оси OY.

// Keys: ? ? ? ?

void MoveX(double value)
{
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		roofPoints3D[i].point.x += value;
	}
}

void MoveY(double value)
{
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		roofPoints3D[i].point.y -= value;
	}
}

#pragma endregion

#pragma region (3) Отражение относительно координатных осей и прямой Y = X.

// Keys: X Y L

void ReflectX()
{
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		roofPoints3D[i].point.x -= screenCentre.x;
		roofPoints3D[i].point.x *= -1;
		roofPoints3D[i].point.x += screenCentre.x;
	}
}

void ReflectY()
{
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		roofPoints3D[i].point.y -= screenCentre.y;
		roofPoints3D[i].point.y *= -1;
		roofPoints3D[i].point.y += screenCentre.y;
	}
}

void ReflectXY()
{
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		std::swap(roofPoints3D[i].point.x, roofPoints3D[i].point.y);
	}
}

#pragma endregion

#pragma region (4) Масштабирование.

// Keys: + -

void Resize(double multiplier)
{
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		roofPoints3D[i].point.x -= screenCentre.x;
		roofPoints3D[i].point.y -= screenCentre.y;
		//
		roofPoints3D[i].point.x *= multiplier;
		roofPoints3D[i].point.y *= multiplier;
		roofPoints3D[i].point.z *= multiplier;
		//
		roofPoints3D[i].point.x += screenCentre.x;
		roofPoints3D[i].point.y += screenCentre.y;
	}
}

#pragma endregion

#pragma region (5) Восстановление исходной позиции фигуры.

// Keys: R

void Reset()
{
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		roofPoints3D[i].point.x = roofTemplate[i][0] * roofTemplateMultiplier;
		roofPoints3D[i].point.y = roofTemplate[i][1] * roofTemplateMultiplier;
		roofPoints3D[i].point.z = roofTemplate[i][2] * roofTemplateMultiplier;
		//
		roofPoints3D[i].point.x += screenCentre.x;
		roofPoints3D[i].point.y += screenCentre.y;
	}
}

#pragma endregion

#pragma region (6) Отсечение невидимых граней.

bool IsVisibleRectangle(int faceIndex)
{
	double sum = 0;
	//
	for (int i = 0, j; i < ROOF_RECTANGLE_VERTEXES; i++)
	{
		j = (ROOF_RECTANGLE_VERTEXES - 1 == i) ? 0 : (i + 1);
		sum += (roofPoints2D[rectangleVerges[faceIndex][i]].point.x - roofPoints2D[rectangleVerges[faceIndex][j]].point.x)
			* (roofPoints2D[rectangleVerges[faceIndex][i]].point.y + roofPoints2D[rectangleVerges[faceIndex][j]].point.y);
	}
	//
	return sum > 0;
}

bool IsVisibleTriangle(int faceIndex)
{
	double sum = 0;
	//
	for (int i = 0, j; i < ROOF_TRIANGLE_VERTEXES; i++)
	{
		j = (ROOF_TRIANGLE_VERTEXES - 1 == i) ? 0 : (i + 1);
		sum += (roofPoints2D[triangleVerges[faceIndex][i]].point.x - roofPoints2D[triangleVerges[faceIndex][j]].point.x)
			* (roofPoints2D[triangleVerges[faceIndex][i]].point.y + roofPoints2D[triangleVerges[faceIndex][j]].point.y);
	}
	//
	return sum > 0;
}

#pragma endregion

#pragma region Отображение фигуры на плоскости и закраска.

double ctn = cos(atan(2.0)) / 2, stn = sin(atan(2.0)) / 2;

void Project()
{
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		roofPoints2D[i].point.x = (long)floor(roofPoints3D[i].point.x - roofPoints3D[i].point.z * ctn);
		roofPoints2D[i].point.y = (long)floor(roofPoints3D[i].point.y - roofPoints3D[i].point.z * stn);
	}
}

double GetLength(VECTOR3D* vector) { return sqrt(pow(vector->x, 2) + pow(vector->y, 2) + pow(vector->z, 2)); }

void Normalize(VECTOR3D* vector)
{
	double length = GetLength(vector);
	//
	if (length == 0)
	{
		vector->x = vector->y = vector->z = 0;
		return;
	}
	//
	vector->x /= length;
	vector->y /= length;
	vector->z /= length;
}

VECTOR3D GetNormalized(VECTOR3D* vector)
{
	VECTOR3D normalized;
	//
	normalized.x = vector->x;
	normalized.y = vector->y;
	normalized.z = vector->z;
	//
	Normalize(&normalized);
	//
	return normalized;
}

double const kd = 0.9; // Luminancy - яркость источника [0, 1]
double const ambientLighting = 0.8; // Рассеянный свет [0, 1]
double K = 0.1; // Изменение интенсивности с расстоянием от источника [0, 1]
double ks = 0.5;

double GetLightIntensity(int pointIndex)
{
	VECTOR3D lightSource, observationPoint, lightVector, vertexPoint, vertexNormal;
	lightSource.x = 0, lightSource.y = 0, lightSource.z = 1; // Координаты источника света (x,y,z : 0-1)
	observationPoint.x = 0, observationPoint.y = 0, observationPoint.z = 0.1; // Точка наблюдения
	//
	// Координаты вершины, для которой вычисляется освещенность.
	vertexPoint = GetNormalized(&roofPoints3D[pointIndex].point);
	//
	// Вектор от вершины на источник света.
	lightVector.x = (lightSource.x - vertexPoint.x);
	lightVector.y = (lightSource.y - vertexPoint.y);
	lightVector.z = -(lightSource.z - vertexPoint.z);
	//
	Normalize(&lightVector);
	//
	vertexNormal = roofPoints3D[pointIndex].normal;
	//
	double cosPhi = lightVector.x * vertexNormal.x + lightVector.y * vertexNormal.y + lightVector.z * vertexNormal.z;
	double cosAlpha = -lightVector.x * observationPoint.x - lightVector.y * observationPoint.y - -lightVector.z * observationPoint.z;
	//
	double d = GetLength(&lightVector);
	double lightIntensity = ambientLighting + (kd * cosPhi + ks * pow(cosAlpha, 3)) / (d + K);
	//
	return (lightIntensity < 0) ? 0 : min(lightIntensity, 1);
}

int Compare(const void *A, const void *B)
{
	auto a = (*(VERTEX2D*)A);
	auto b = (*(VERTEX2D*)B);
	//
	return a.point.y > b.point.y ? 1 : (a.point.y == b.point.y) ? 0 : -1;
}

// Создание массива точек линии строго с отличающейся координатой Y.
void CreateLine(VERTEX2D p1, VERTEX2D p2)
{
	int dx = abs(p2.point.x - p1.point.x), dy = abs(p2.point.y - p1.point.y);
	int sx = p1.point.x < p2.point.x ? 1 : -1, sy = p1.point.y < p2.point.y ? 1 : -1;
	double t, len = sqrt(double(dx*dx + dy*dy)), dxc, dyc;
	int x = p1.point.x, y = p1.point.y, prev = INT_MIN;
	int error = dx - dy, err;
	while (true)
	{
		// Вычисление интенсивности в точке линии.
		dxc = p1.point.x - x;
		dyc = p1.point.y - y;
		t = sqrt(dxc * dxc + dyc * dyc) / len;
		err = error << 1;
		//
		if (y != prev)
		{
			pointList[pointListCount].point.x = x;
			pointList[pointListCount].point.y = y;
			pointList[pointListCount].light = (1 - t) * p1.light + t * p2.light;
			pointListCount++;
		}
		//
		prev = y;
		//
		if (x == p2.point.x && y == p2.point.y)
		{
			break;
		}
		//
		if (err > -dy)
		{
			error -= dy;
			x += sx;
		}
		//
		if (err < dx)
		{
			pointList[pointListCount].point.x = x;
			pointList[pointListCount].point.y = y;
			pointList[pointListCount].light = (1 - t) * p1.light + t * p2.light;
			pointListCount++;
			error += dx, y += sy;
		}
	}
}

void SetPixel(DWORD* pixels, int x, int y, COLORREF color)
{
	if (x < 0 || y < 0 || x > clientRectangle.right - 1 || y > clientRectangle.bottom - 1)
	{
		return;
	}
	//
	pixels[x + y * clientRectangle.right] = GetRValue(color) << 16 | (WORD)GetGValue(color) << 8 | GetBValue(color);
}

void FillClientRectangle(DWORD* pixels, int x1, int y1, int x2, int y2, COLORREF color)
{
	int dx = abs(x2 - x1) + 1, dy = abs(y2 - y1) + 1, i;
	//
	if (min(x1, x2) + dx >= clientRectangle.right)
	{
		dx = clientRectangle.right - min(x1, x2);
	}
	if (min(y1, y2) + dy >= clientRectangle.bottom)
	{
		dy = clientRectangle.bottom - min(y1, y2);
	}
	//
	color = RGB(255, 255, 255);
	DWORD * line = new DWORD[dx];
	//
	for (i = 0; i < dx; i++)
	{
		line[i] = color;
	}
	for (i = 0; i < dy; i++)
	{
		memcpy(&pixels[min(x1, x2) + (min(y1, y2) + i)*clientRectangle.right], line, sizeof(DWORD)*dx);
	}
	//
	delete line;
}

void FillRectangleFaceWithColor(PDWORD pixels, int faceIndex, int vertexCount)
{
	int i, next, x, y, x1, x2;
	double I, I2, incr;
	//
	pointListCount = 0;
	//
	// Создание растрового массива точек контура с учетом освещенности.
	for (i = 0; i < vertexCount; i++)
	{
		// Добавление в массив точек очередного ребра полигона.
		next = i != vertexCount - 1 ? i + 1 : 0;
		CreateLine(roofPoints2D[rectangleVerges[faceIndex][i]], roofPoints2D[rectangleVerges[faceIndex][next]]);
	}
	//
	// Сортировка точек по Y.
	qsort(pointList, pointListCount, sizeof(VERTEX2D), Compare);
	//
	// Закрашивание грани.
	for (i = 0; i < pointListCount - 1; i++)
	{
		y = pointList[i].point.y;
		//
		if (y != pointList[i + 1].point.y)
		{
			continue;
		}
		//
		x1 = pointList[i].point.x, x2 = pointList[i + 1].point.x;
		I = pointList[i].light, I2 = pointList[i + 1].light;
		//
		// Отрисовка горизонтальной линии.
		if (x1 > x2)
		{
			swap(x1, x2);
			swap(I, I2);
		}
		//
		incr = (I2 - I) / (x2 - x1);
		//
		for (x = x1; x <= x2; x++)
		{
			auto colour = RGB(rectangleColors[faceIndex][0] * (I), rectangleColors[faceIndex][1] * (I), rectangleColors[faceIndex][2] * (I));
			SetPixel(pixels, x, y, colour);
			I += incr; // Интерполяция интенсивности.
		}
	}
}

void FillTriangleFaceWithColor(PDWORD pixels, int faceIndex, int vertexCount)
{
	int i, next, x, y, x1, x2;
	double I, I2, incr;
	//
	pointListCount = 0;
	//
	// Создание растрового массива точек контура с учетом освещенности.
	for (i = 0; i < vertexCount; i++)
	{
		// Добавление в массив точек очередного ребра полигона.
		next = i != vertexCount - 1 ? i + 1 : 0;
		CreateLine(roofPoints2D[triangleVerges[faceIndex][i]], roofPoints2D[triangleVerges[faceIndex][next]]);
	}
	//
	// Сортировка точек по Y.
	qsort(pointList, pointListCount, sizeof(VERTEX2D), Compare);
	//
	// Закрашивание грани.
	for (i = 0; i < pointListCount - 1; i++)
	{
		y = pointList[i].point.y;
		//
		if (y != pointList[i + 1].point.y)
		{
			continue;
		}
		//
		x1 = pointList[i].point.x, x2 = pointList[i + 1].point.x;
		I = pointList[i].light, I2 = pointList[i + 1].light;
		//
		// Отрисовка горизонтальной линии.
		if (x1 > x2)
		{
			swap(x1, x2);
			swap(I, I2);
		}
		//
		incr = (I2 - I) / (x2 - x1);
		//
		for (x = x1; x <= x2; x++)
		{
			auto colour = RGB(triangleColors[faceIndex][0] * (I), triangleColors[faceIndex][1] * (I), triangleColors[faceIndex][2] * (I));
			SetPixel(pixels, x, y, colour);
			I += incr;
		}
	}
}

void ShowRoof(PDWORD pixels, int width, int height)
{
	// Вычисление интенсивности во всех точках.
	for (int j = 0; j < ROOF_POINTS; j++)
	{
		roofPoints2D[j].light = GetLightIntensity(j);
	}
	// Закрашивание граней.
	for (int i = 0; i < ROOF_RECTANGLE_FACES; i++)
	{
		if (IsVisibleRectangle(i))
		{
			FillRectangleFaceWithColor(pixels, i, ROOF_RECTANGLE_VERTEXES);
		}
	}
	//
	for (int i = 0; i < ROOF_TRIANGLE_FACES; i++)
	{
		if (IsVisibleTriangle(i))
		{
			FillTriangleFaceWithColor(pixels, i, ROOF_TRIANGLE_VERTEXES);
		}
	}
}

#pragma endregion

VECTOR3D Subtract(VECTOR3D t1, VECTOR3D t2)
{
	VECTOR3D summ;
	summ.x = t1.x - t2.x;
	summ.y = t1.y - t2.y;
	summ.z = t1.z - t2.z;
	return summ;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; // Store instance handle in our global variable

	HMENU hMenu = CreateMenu();
	
	HMENU hSubMenu = CreatePopupMenu();
	AppendMenu(hSubMenu, MF_STRING, ROTATE_X_ITEM, L"Rotate X (X)");
	AppendMenu(hSubMenu, MF_STRING, ROTATE_Y_ITEM, L"Rotate Y (Y)");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"Rotate");
	
	hSubMenu = CreatePopupMenu();
	AppendMenu(hSubMenu, MF_STRING, MOVE_LEFT_ITEM, L"Move Left (Left Arrow)");
	AppendMenu(hSubMenu, MF_STRING, MOVE_RIGHT_ITEM, L"Move Right (Right Arrow)");
	AppendMenu(hSubMenu, MF_STRING, MOVE_UP_ITEM, L"Move Up (Up Arrow)");
	AppendMenu(hSubMenu, MF_STRING, MOVE_DOWN_ITEM, L"Move Down (Down Arrow)");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"Move");
	
	hSubMenu = CreatePopupMenu();
	AppendMenu(hSubMenu, MF_STRING, REFLECT_X_ITEM, L"Reflect X (X)");
	AppendMenu(hSubMenu, MF_STRING, REFLECT_Y_ITEM, L"Reflect Y (Y)");
	AppendMenu(hSubMenu, MF_STRING, REFLECT_XY_ITEM, L"Reflect l = XY (L)");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"Reflect");

	hSubMenu = CreatePopupMenu();
	AppendMenu(hSubMenu, MF_STRING, RESIZE_UP_ITEM, L"Increase Size (+)");
	AppendMenu(hSubMenu, MF_STRING, RESIZE_DOWN_ITEM, L"Decrease Size (-)");
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"Resize");

	AppendMenu(hMenu, MF_STRING, RESET_ITEM, L"Reset");

	// Main window.
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 380 * 2, 380 * 2, nullptr, hMenu, hInstance, nullptr);
	if (hWnd == nullptr)
	{
		return false;
	}

	// Подготовка координат.
	GetClientRect(hWnd, &clientRectangle);
	screenCentre.x = clientRectangle.right / 2;
	screenCentre.y = clientRectangle.bottom / 2;
	//
	for (int i = 0; i < ROOF_POINTS; i++)
	{
		roofPoints3D[i].point.x = roofTemplate[i][0] * roofTemplateMultiplier;
		roofPoints3D[i].point.y = roofTemplate[i][1] * roofTemplateMultiplier;
		roofPoints3D[i].point.z = roofTemplate[i][2] * roofTemplateMultiplier;
		//
		roofPoints3D[i].point.x += screenCentre.x;
		roofPoints3D[i].point.y += screenCentre.y;
	}
	// Вычисление нормалей во всех вершинах.
	VECTOR3D v1, v2;
	for (int j = 0; j < ROOF_POINTS; j++)
	{
		ZeroMemory(&roofPoints3D[j].normal, sizeof VECTOR3D);
		//
		for (int i = 0; i < ROOF_RECTANGLE_FACES; i++)
		{
			for (int k = 0; k < ROOF_RECTANGLE_VERTEXES; k++)
			{
				if (rectangleVerges[i][k] == j)
				{
					v1 = Subtract(roofPoints3D[rectangleVerges[i][0]].point, roofPoints3D[rectangleVerges[i][1]].point);
					v2 = Subtract(roofPoints3D[rectangleVerges[i][2]].point, roofPoints3D[rectangleVerges[i][1]].point);
					//
					roofPoints3D[j].normal.x += v1.y * v2.z - v1.z * v2.y;
					roofPoints3D[j].normal.y += v1.z * v2.x - v1.x * v2.z;
					roofPoints3D[j].normal.z += v1.x * v2.y - v1.y * v2.x;
				}
			}
		}
		Normalize(&roofPoints3D[j].normal);
	}
	//
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	SetTimer(hWnd, NULL, 10, NULL);
	//
	return true;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	- Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
WPARAM key = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	double alpha = 0.05;
	double value = 5;
	//
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ROTATE_X_ITEM:
			RotateX(alpha, (rotationPoint.x && rotationPoint.y) ? rotationPoint : screenCentre);
			break;
		case ROTATE_Y_ITEM:
			RotateY(alpha, (rotationPoint.x && rotationPoint.y) ? rotationPoint : screenCentre);
			break;
		case MOVE_LEFT_ITEM:
			MoveX(-value);
			break;
		case MOVE_RIGHT_ITEM:
			MoveX(value);
			break;
		case MOVE_UP_ITEM:
			MoveY(value);
			break;
		case MOVE_DOWN_ITEM:
			MoveY(-value);
			break;
		case REFLECT_X_ITEM:
			ReflectX();
			break;
		case REFLECT_Y_ITEM:
			ReflectY();
			break;
		case REFLECT_XY_ITEM:
			ReflectXY();
			break;
		case RESIZE_UP_ITEM:
			Resize(1.2);
			break;
		case RESIZE_DOWN_ITEM:
			Resize(0.8);
			break;
		case RESET_ITEM:
			Reset();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		//
	case WM_CREATE:
		// Создание буфера для построения изображения.
		ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = clientRectangle.right;
		bmi.bmiHeader.biHeight = -clientRectangle.bottom;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32; /* Request a 32bpp bitmap. */
		bmi.bmiHeader.biCompression = BI_RGB;
		pixels = new DWORD[clientRectangle.right * clientRectangle.bottom];
		break;
		//
	case WM_SIZE:
		if (pixels)
		{
			delete pixels;
		}
		//
		for (int i = 0; i < ROOF_POINTS; i++)
		{
			roofPoints3D[i].point.x -= screenCentre.x;
			roofPoints3D[i].point.y -= screenCentre.y;
		}
		//
		GetClientRect(hWnd, &clientRectangle);
		//
		screenCentre.x = clientRectangle.right / 2;
		screenCentre.y = clientRectangle.bottom / 2;
		//
		for (int i = 0; i < ROOF_POINTS; i++)
		{
			roofPoints3D[i].point.x += screenCentre.x;
			roofPoints3D[i].point.y += screenCentre.y;
		}
		//
		pixels = new DWORD[clientRectangle.right * clientRectangle.bottom];
		//
		bmi.bmiHeader.biWidth = clientRectangle.right;
		bmi.bmiHeader.biHeight = -clientRectangle.bottom;
		//
		break;
		//
	case WM_LBUTTONDOWN:
		//KillTimer(hWnd, NULL);
		rotationPoint.x = LOWORD(lParam);
		rotationPoint.y = HIWORD(lParam);
		break;
		//
	case WM_LBUTTONUP:
		//SetTimer(hWnd, NULL, 10, NULL);
		break;
		//
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			MoveX(-value);
			break;
		case VK_RIGHT:
			MoveX(value);
			break;
		case VK_UP:
			MoveY(value);
			break;
		case VK_DOWN:
			MoveY(-value);
			break;
		default:
			break;
		}
		break;
		//
	case WM_CHAR:
		// Управление только для латинской раскладки!
		switch (wParam)
		{
		case 'W':
		case 'w':
		case 'Ц' + RUSSIAN_KEY_OFFSET:
		case 'ц' + RUSSIAN_KEY_OFFSET:
			RotateX(alpha, (rotationPoint.x && rotationPoint.y) ? rotationPoint : screenCentre);
			break;
		case 'A':
		case 'a':
		case 'Ф' + RUSSIAN_KEY_OFFSET:
		case 'ф' + RUSSIAN_KEY_OFFSET:
			RotateY(-alpha, (rotationPoint.x && rotationPoint.y) ? rotationPoint : screenCentre);
			break;
		case 'S':
		case 's':
		case 'Ы' + RUSSIAN_KEY_OFFSET:
		case 'ы' + RUSSIAN_KEY_OFFSET:
			RotateX(-alpha, (rotationPoint.x && rotationPoint.y) ? rotationPoint : screenCentre);
			break;
		case 'D':
		case 'd':
		case 'В' + RUSSIAN_KEY_OFFSET:
		case 'в' + RUSSIAN_KEY_OFFSET:
			RotateY(alpha, (rotationPoint.x && rotationPoint.y) ? rotationPoint : screenCentre);
			break;
		case 'X':
		case 'x':
		case 'Ч' + RUSSIAN_KEY_OFFSET:
		case 'ч' + RUSSIAN_KEY_OFFSET:
			ReflectX();
			break;
		case 'Y':
		case 'y':
		case 'Н' + RUSSIAN_KEY_OFFSET:
		case 'н' + RUSSIAN_KEY_OFFSET:
			ReflectY();
			break;
		case 'L':
		case 'l':
		case 'Д' + RUSSIAN_KEY_OFFSET:
		case 'д' + RUSSIAN_KEY_OFFSET:
			ReflectXY();
			break;
		case '+':
		case '=':
			Resize(1.2);
			break;
		case '-':
			Resize(0.8);
			break;
		case 'R':
		case 'r':
		case 'К' + RUSSIAN_KEY_OFFSET:
		case 'к' + RUSSIAN_KEY_OFFSET:
			Reset();
			break;
		default:
			break;
		}
		break;
		//
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (pixels == nullptr) { break; }
		// Отрисовка в массиве pixels.
		FillClientRectangle(pixels, 0, 0, clientRectangle.right, clientRectangle.bottom, RGB(100, 100, 100));
		// Проецирование.
		Project();
		// Отображение.
		ShowRoof(pixels, clientRectangle.right, clientRectangle.bottom);
		SetDIBitsToDevice(hdc, 0, 0, clientRectangle.right, clientRectangle.bottom, 0, 0, 0, clientRectangle.bottom, pixels, &bmi, DIB_RGB_COLORS);
		EndPaint(hWnd, &ps);
		break;
		//
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
		break;
		//
	case WM_ERASEBKGND:
		break;
		//
	case WM_DESTROY:
		if (pixels)
		{
			delete pixels;
		}
		PostQuitMessage(0);
		break;
		//
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
