#pragma once
#include <iostream>
#include "TypeList.h"

class TValue
{
public:

	/// <summary>
	/// ��� ��������� ������ ��� ���������� ��� ������.
	/// </summary>
	/// <param name="value">�������� ��� ����������, �������� ��� ��������� ��� �������.</param>
	/// <param name="type">��� ���������� ��� ��� ��������� �������.</param>
	/// <param name="size">������ �������, 0 ��� ����������</param>
	TValue(double value, int type, int size = 0);

	/// <summary>
	/// �������� TValue ��� ���������� � ��� ���������� ������.
	/// </summary>
	/// <param name="pointer">��������� ��� ���������� ������.</param>
	/// <param name="type">��� ����������, �� ������� ��������� pointer, ��� ��� ��������� �������.</param>
	TValue(double* pointer, int type);

	/// <summary>
	/// �������� ��������� �� nullptr ���� void*.
	/// </summary>
	TValue();
	
	~TValue();

	/// <summary>
	/// �������� ��� ���������� ��� �������.
	/// </summary>
	int GetType();

	/// <summary>
	/// �������� �������� ���������� ��� �������� �������.
	/// </summary>
	/// <param name="index">������ �������� �������, ��� ���������� �� �����������.</param>
	double GetValue(int index = 0);

	/// <summary>
	/// ������������� �������� ���������� ��� �������� �������.
	/// </summary>
	/// <param name="v">����� �������� ���������� ��� �������� �������.</param>
	/// <param name="index">������ �������� �������, ��� ���������� �� �����������.</param>
	void SetValue(double v, int index = 0);

	/// <summary>
	/// �������� ����� ���������� ��� ������� �������� �������.
	/// </summary>
	double* GetPointer();

	/// <summary>
	/// ������������� ����� ���������� ��� ������� �������� �������.
	/// </summary>
	/// <param name="pointer">����� ���������� ��� �������.</param>
	void SetPointer(double*);

	/// <summary>
	/// ���������� ����������, �������� ������� �������.
	/// </summary>
	/// <param name="pointer">������ �������� �������.</param>
	TValue* GetElementTValue(int index);

	bool IsReference();

	void Print();

private:
	int type;
	double* pointer;
	bool referenceType;
};