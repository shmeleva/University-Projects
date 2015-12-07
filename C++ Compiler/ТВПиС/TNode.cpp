#include "stdafx.h"
#include "TNode.h"
#include "UnaryOperationList.h"
#include "BinaryOperationList.h"
#include "Exception.h"

#include <iostream>

using namespace std;

// 1. ���������.

TConst::TConst(TValue* c)
{
	_const = c;
}

TConst::~TConst()
{
	if (_const) delete _const;
}

TValue* TConst::GetTValue()
{
	return _const;
}

double TConst::GetValue()
{
	return _const->GetValue();
}

int TConst::GetType()
{
	return _const->GetType();
}

TValue* TConst::Execute()
{
	return _const;
}

// 2. ����������.

TVariable::TVariable(TValue* v)
{
	_variable = v;
}

TVariable::~TVariable()
{
	if (_variable) delete _variable;
}

TValue* TVariable::GetTValue()
{
	return _variable;
}

double TVariable::GetValue()
{
	return _variable->GetValue();
}

void TVariable::SetValue(double value)
{
	_variable->SetValue(value);
}

int TVariable::GetType()
{
	return _variable->GetType();
}

TValue* TVariable::Execute()
{
	return _variable;
}

// 3. ������.

TArray::TArray(TValue* array, TNode* index)
{
	_array = array;
	_index = index;
}

TArray::~TArray()
{
	if (_array) delete _array;
	_array = nullptr;

	if (_index) delete _index;
	_index = nullptr;
}

TValue* TArray::Execute()
{
	return _array->GetElementTValue((int)_index->Execute()->GetValue());
}

// 4. ������� ��������.

TUnaryOperation::TUnaryOperation(int type, TNode* operand)
{
	if (UnaryOperationList::Instance().GetOperationName(type))
	{
		_type = type;
		_operand = operand;
	}
	else
	{
		throw new Exception("UnknownOperation: ���������������� ��������.");
	}
}

TUnaryOperation::~TUnaryOperation()
{
	if (_operand) delete _operand;
}

TValue* TUnaryOperation::Execute()
{
	TValue* result = _operand->Execute();

	char* operation = UnaryOperationList::Instance().GetOperationName(_type);

	if (result->IsReference() && strcmp(operation, "output"))
	{
		// �������� ���������� ��� ��������, �� �� ��� ����������.
		// �� ������ (������� �����) ����� �������� output
		throw new Exception("InvalidOperation: ������������ ��������.");
	}

	// ������� ����
	if (strcmp(operation, "+") == 0)
	{
		return new TValue(result->GetValue(), result->GetType());
	}

	// ������� �����
	if (strcmp(operation, "-") == 0)
	{
		return new TValue(-(result->GetValue()), result->GetType());
	}

	// ���������� �� � ��������� ���������
	if (strcmp(operation, "!") == 0)
	{
		return new TValue(!(result->GetValue()), result->GetType());
	}

	// ����������� ���������
	if (strcmp(operation, "�++") == 0)
	{
		double temp = result->GetValue();
		result->SetValue(temp + 1);
		return new TValue(temp, result->GetType());
	}

	// ����������� ���������
	if (strcmp(operation, "�--") == 0)
	{
		double temp = result->GetValue();
		result->SetValue(temp - 1);
		return new TValue(temp, result->GetType());
	}

	// ���������� ���������
	if (strcmp(operation, "++�") == 0)
	{
		double temp = result->GetValue() + 1;
		result->SetValue(temp);
		return new TValue(temp, result->GetType());
	}

	// ���������� ���������
	if (strcmp(operation, "--�") == 0)
	{
		double temp = result->GetValue() - 1;
		result->SetValue(temp);
		return new TValue(temp, result->GetType());
	}

	// ����
	if (strcmp(operation, "input") == 0)
	{
		double temp;
		std::cin >> temp;
		
		return new TValue(temp, TypeList::Instance().GetTypeIndex("double"));
	}

	// �����
	if (strcmp(operation, "output") == 0)
	{
		if (!result->IsReference())
		{
			
			char* typeName = TypeList::Instance().GetTypeName(result->GetType());

			if (!strcmp(typeName, "double"))
			{
				cout << result->GetValue();
			}
			else if (!strcmp(typeName, "float"))
			{
				cout << (float)result->GetValue();
			}
			else if (!strcmp(typeName, "int"))
			{
				cout << (int)result->GetValue();
			}
			else if (!strcmp(typeName, "bool"))
			{
				cout << (bool)result->GetValue();
			}
			else if (!strcmp(typeName, "char"))
			{
				cout << (char)result->GetValue();
			}
			
		}
		else
		{
			double* start = result->GetPointer();

			char i = (char)(*start++);

			while (i)
			{
				cout << i;
				i = (char)(*start++);
			}
		}

		return nullptr;
	}

	// �����
	if (strcmp(operation, "sin") == 0)
	{
		return new TValue(sin(result->GetValue()), TypeList::Instance().GetTypeIndex("double"));
	}

	// �������
	if (strcmp(operation, "cos") == 0)
	{
		return new TValue(cos(result->GetValue()), TypeList::Instance().GetTypeIndex("double"));
	}

	if (strcmp(operation, "rand") == 0)
	{
		int random = rand();
		return new TValue(random % (int)result->GetValue(), TypeList::Instance().GetTypeIndex("int"));
	}
	
	throw new Exception("UnknownOperation: ���������������� ��������.");
}

// 5. �������� ��������.

TBinaryOperation::TBinaryOperation(int type, TNode* left, TNode* right)
{
	if (BinaryOperationList::Instance().GetOperationName(type))
	{
		_type = type;
		_left = left;
		_right = right;
	}
	else
	{
		throw new Exception("UnknownOperation: ���������������� ��������.");
	}	
}

TBinaryOperation::~TBinaryOperation()
{
	if (_left) delete _left;
	if (_right) delete _right;
}

TValue* TBinaryOperation::Execute()
{
	TValue* leftResult = _left->Execute();
	TValue* rightResult = _right->Execute();
	char* operation = BinaryOperationList::Instance().GetOperationName(_type);

	//���������� ������.
	if (strcmp(operation, "[]") == 0)
	{
		if (leftResult->IsReference() && rightResult->GetType() == TypeList::Instance().GetTypeIndex("int"))
		{
			// ����� TValue ��������� �� ��� ������������ ������� ������.
			return new TValue((leftResult->GetPointer() + (int)rightResult->GetValue()), leftResult->GetType());
		}

		throw new Exception("InvalidOperation: ������������ ��������.");
	}

	if (leftResult->IsReference() || rightResult->IsReference())
	{
		// �������� ���������� ��� ��������, �� �� ��� ����������.
		throw new Exception("InvalidOperation: ������������ ��������.");
	}

	// ������������.
	if (strcmp(operation, "=") == 0)
	{
		double value = rightResult->GetValue();
		leftResult->SetValue(value);
		return new TValue(value, leftResult->GetType());
	}

	// ��������.
	if (strcmp(operation, "+") == 0)
	{
		return new TValue(leftResult->GetValue() + rightResult->GetValue(), leftResult->GetType());
	}

	// ���������.
	if (strcmp(operation, "-") == 0)
	{
		return new TValue(leftResult->GetValue() - rightResult->GetValue(), leftResult->GetType());
	}

	// ���������.
	if (strcmp(operation, "*") == 0)
	{
		return new TValue(leftResult->GetValue() * rightResult->GetValue(), leftResult->GetType());
	}

	// �������.
	if (strcmp(operation, "/") == 0)
	{
		return new TValue(leftResult->GetValue() / rightResult->GetValue(), leftResult->GetType());
	}

	// ������� �� �������.
	if (strcmp(operation, "%") == 0)
	{
		return new TValue((int)leftResult->GetValue() % (int)rightResult->GetValue(), TypeList::Instance().GetTypeIndex("int"));
	}

	// ���������� �.
	if (strcmp(operation, "&&") == 0)
	{
		return new TValue(leftResult->GetValue() && rightResult->GetValue(), TypeList::Instance().GetTypeIndex("bool"));
	}

	// ���������� ���.
	if (strcmp(operation, "||") == 0)
	{
		return new TValue(leftResult->GetValue() || rightResult->GetValue(), TypeList::Instance().GetTypeIndex("bool"));
	}

	// �������� �� ���������.
	if (strcmp(operation, "==") == 0)
	{
		return new TValue(leftResult->GetValue() == rightResult->GetValue(), TypeList::Instance().GetTypeIndex("bool"));
	}

	// �������� �� �����������.
	if (strcmp(operation, "!=") == 0)
	{
		return new TValue(leftResult->GetValue() != rightResult->GetValue(), TypeList::Instance().GetTypeIndex("bool"));
	}
	
	// ������.
	if (strcmp(operation, ">") == 0)
	{
		return new TValue(leftResult->GetValue() > rightResult->GetValue(), TypeList::Instance().GetTypeIndex("bool"));
	}

	// ������.
	if (strcmp(operation, "<") == 0)
	{
		return new TValue(leftResult->GetValue() < rightResult->GetValue(), TypeList::Instance().GetTypeIndex("bool"));
	}

	// ������ ��� �����.
	if (strcmp(operation, ">=") == 0)
	{
		return new TValue(leftResult->GetValue() >= rightResult->GetValue(), TypeList::Instance().GetTypeIndex("bool"));
	}

	// ������ ��� �����.
	if (strcmp(operation, "<=") == 0)
	{
		return new TValue(leftResult->GetValue() <= rightResult->GetValue(), TypeList::Instance().GetTypeIndex("bool"));
	}

	// ��������� �.
	if (strcmp(operation, "&") == 0)
	{
		return new TValue((int)leftResult->GetValue() & (int)rightResult->GetValue(), TypeList::Instance().GetTypeIndex("int"));
	}

	// ��������� ���.
	if (strcmp(operation, "|") == 0)
	{
		return new TValue((int)leftResult->GetValue() | (int)rightResult->GetValue(), TypeList::Instance().GetTypeIndex("int"));
	}

	// ������� ����� �����.
	if (strcmp(operation, "<<") == 0)
	{
		return new TValue((int)leftResult->GetValue() << (int)rightResult->GetValue(), TypeList::Instance().GetTypeIndex("int"));
	}

	// ������� ����� ������.
	if (strcmp(operation, ">>") == 0)
	{
		return new TValue((int)leftResult->GetValue() >> (int)rightResult->GetValue(), TypeList::Instance().GetTypeIndex("int"));
	}

	// �������.
	if (strcmp(operation, "min") == 0)
	{
		return new TValue(leftResult->GetValue() < rightResult->GetValue() ? leftResult->GetValue() : rightResult->GetValue(), leftResult->GetType());
	}

	// ��������.
	if (strcmp(operation, "max") == 0)
	{
		return new TValue(leftResult->GetValue() > rightResult->GetValue() ? leftResult->GetValue() : rightResult->GetValue(), leftResult->GetType());
	}

	return nullptr;
}

// 6. ������ ����������� ��������.

TList::TList(TList* parent)
{
	this->first = nullptr;
	this->scope = parent ? new Scope(parent->GetScope()) : new Scope();
}

TList::~TList()
{
	while (first)
	{
		ListNode* clear = first;
		first = first->_next;
		delete clear;
		clear = nullptr;
	}

	if (scope) delete scope;
	scope = nullptr;
}

TValue* TList::Execute()
{
	ListNode* current = first;
	while (current)
	{
		current->_expression->Execute();
		current = current->_next;
	}

	return nullptr;
}
	
void  TList::Push(TNode* node)
{
	ListNode* _node = new ListNode();
	_node->_expression = node;
	_node->_next = nullptr;

	if (first == nullptr)
	{
		first = _node;
		return;
	}

	ListNode* current = first;
	while (current->_next)
	{
		current = current->_next;
	}
	current->_next = _node;
}

Scope* TList::GetScope()
{
	return scope;
}

// 7. ����������� IF-ELSE.

TIf::TIf(TNode* _condition, TList* _then, TList* _else)
{
	this->_condition = _condition;
	this->_then = _then;
	this->_else = _else;
}

TIf::~TIf()
{
	if (_condition) delete _condition;
	if (_then) delete _then;
	if (_else) delete _else;
}

TValue* TIf::Execute()
{
	return (_condition->Execute()->GetValue()) ? _then->Execute() : (_else ? _else->Execute() : nullptr); // ������ nullptr.
}

// 8. ����������� WHILE.

TWhile::TWhile(TNode* _condition, TList* _body)
{
	this->_condition = _condition;
	this->_body = _body;
}

TWhile::~TWhile()
{
	if (_condition) delete _condition;
	if (_body) delete _body;
}

TValue* TWhile::Execute()
{
	while (_condition->Execute()->GetValue())
	{
		_body->Execute();
	}

	return nullptr;
}

