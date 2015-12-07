#include "stdafx.h"
#include "UnaryOperationList.h"
#include <cstring>
#include "defaults.h"

UnaryOperationList::UnaryOperationList(void)
{
	operations = new List(MAX_OPERATION_NAME_LENGHT + 1);
	LoadOperations();
}


UnaryOperationList::~UnaryOperationList(void)
{
	delete operations;
}

UnaryOperationList& UnaryOperationList::Instance()
{
	static UnaryOperationList _instance;
	return _instance;
}

void UnaryOperationList::LoadOperations()
{
	operations->Add((void*)"+");		// ������� ���� 
	operations->Add((void*)"-");		// ������� �����

	operations->Add((void*)"!");		// ���������� �� � ��������� ���������

	operations->Add((void*)"�++");		// ����������� ���������
	operations->Add((void*)"�--");		// ����������� ���������
	operations->Add((void*)"++�");		// ���������� ���������
	operations->Add((void*)"--�");		// ���������� ���������

	operations->Add((void*)"input");	// ����
	operations->Add((void*)"output");	// �����

	operations->Add((void*)"sin");		// �����
	operations->Add((void*)"cos");		// �������
	operations->Add((void*)"rand");		// �������
}

bool UnaryOperationList::IsOperation(char* name)
{
	return GetOperationIndex(name) < 0 ? false : true;
}

int UnaryOperationList::GetOperationIndex(char* name)
{
	for (int i = 0; i < operations->Count(); i++)
		if (strcmp(name, (char*)operations->GetAccessor(i)) == 0)
			return i;

	return -1;
}

char* UnaryOperationList::GetOperationName(int index)
{
	if (index < 0 || index >= operations->Count())
		return nullptr;

	char* name = (char*)operations->GetAccessor(index), *copy = new char[strlen(name) + 1];
	Copy(copy, name, strlen(name));

	return copy;
}
