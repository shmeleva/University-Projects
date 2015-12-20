#pragma once

#include "TValue.h"
#include "TypeList.h"
#include "OperationList.h"
#include "Scope.h"
#include <iostream>

// ������� ����� ��� ���� ����� � ������� ��������������� ������.
class TNode
{
public:
	virtual TValue* Execute() = 0;
};

// ���������.
class TConst : public TNode
{
public:
	TConst(TValue* c);
	~TConst();
	TValue* GetTValue();

	double GetValue();
	int GetType();
	TValue* Execute();

protected:
	TValue* _const;
};

// ����������.
class TVariable : public TNode
{
public:
	TVariable(TValue* v);
	~TVariable();
	TValue* GetTValue();

	double GetValue();
	void SetValue(double);
	int GetType();
	TValue* Execute();

protected:
	TValue* _variable;
};

// ������.
class TArray : public TNode
{
public:
	TArray(TValue* array, TNode* index);
	~TArray();

	TValue* Execute();

protected:
	TValue* _array;
	TNode* _index;
};

// ������� ��������.
class TUnaryOperation : public TNode
{
public:
	TUnaryOperation(int, TNode*);
	~TUnaryOperation();

	TValue* Execute();

protected:
	int      _type;		// # � ������� ��������
	TNode*   _operand;
};

// �������� ��������.
class TBinaryOperation : public TNode
{
public:
	TBinaryOperation(int, TNode*, TNode*);
	~TBinaryOperation();

	TValue* Execute();

protected:
	int     _type;
	TNode*   _left;
	TNode*   _right;
};

// ������ ����������� ��������.
class TList : public TNode
{
	struct ListNode
	{
		TNode* _expression;
		ListNode* _next;
	};

public:
	TList(TList* parent = nullptr);
	~TList();

	TValue* Execute();
	void  Push(TNode*);

	Scope* GetScope();

protected:
	ListNode* first;
	Scope* scope;
};

// ����������� IF-ELSE.
class TIf : public TNode
{
public:
	TIf(TNode*, TList*, TList*);
	~TIf();

	TValue* Execute();

protected:
	TNode* _condition;
	TList* _then;
	TList* _else;
};

// ����������� WHILE.
class TWhile : public TNode
{
public:
	TWhile(TNode*, TList*);
	~TWhile();

	TValue* Execute();

protected:
	TNode* _condition;
	TList* _body;
};