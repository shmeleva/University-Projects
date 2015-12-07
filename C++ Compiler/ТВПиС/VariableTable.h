#pragma once
#include "Hash.h"
#include "TValueKeeper.h"

// ���-������� ��� �������� �������� TValue
// � ���� �������� � ���������� �� ��������.

class VariableTable : public Hash
{
public:
	VariableTable() : Hash(9, 0, 0, 0, 0) {  }

	~VariableTable();

	unsigned int Key1(char*);

	TValueKeeper* Find(char*);

	TValueKeeper* Push(TValueKeeper*);
	TValueKeeper* Push(char* name, double value, int type, int size = 0);
};

