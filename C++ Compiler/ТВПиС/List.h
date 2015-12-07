#pragma once
#include "Heap.h"
#include "Defaults.h"
#include "SortingMethod.h"

class List
{
public:

	List(int elementSize, int elementCount = LISTSIZE);
	~List();

	void* Get(int);		// ���������� � ���������� �� ������
	void* Add(void*);	// Push � ����� ������

	// ���������� �������� � void* � ���������� �� ������
	void TakeFirst(void*);
	void TakeLast(void*);
	void Take(int, void*);

	void Sort(bool dir = true, SortingMethod method = SortingMethod::QuickSort);

	int Count();
	bool Error();		// true if error in last operation

	void SetAccessor(void*, int);
	void* GetAccessor(int);
protected:

	void Swap(int, int);

	

	struct Segment
	{
		void*    data;
		Segment* prev;
		Segment* next;
	};

	Segment* NewSegment();
	Segment* GetSegment(int);
	void DeleteSegment(Segment*);

	void CopyElement(void*, void*);

	bool IsFree(Segment*);

private:

	Segment* first;
	Segment* last;

	int firstIndex;		//����� ������� ��������
	int lastIndex;		//����� ���������� ��������

	int elementSize;	//������ ������ �������� � ������
	bool error;			//true, ���� �������� ������, ��������, ��� ��������� ������
	int segmentCount;	//������� ���������� ���������
	int elementCount;	//���������� ��������� �� ���� �������
};