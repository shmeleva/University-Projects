#include "stdafx.h"
#include "ASTBuilder.h"

ASTBuilder::ASTBuilder(char* sourceCode)
{
	Automat* automat = new Automat(AUTOMAT);
	FileReader* fileReader = new FileReader(sourceCode);

	tokens = automat->Work(fileReader->ReadSourceCode());

	if (automat) delete automat;
	if (fileReader) delete fileReader;

	// ���������� ������ ������ � ������ ����.
	tokens = OldTokenToNewToken(tokens);

	//PrintList(tokens);

	// ����������� ����������� ��������.
	Prioritize(tokens);

	astRoot = currentList = nullptr;
	currentScope = nullptr;
}

// ������ ��������� ������.
// @ ����� ������� Run() ���������� ����� ��������� Build().
// Build() �� ����� ������� ����������, �� ���� ����������� ������������ ������
// ��� ����� �� �������������� ��������� (��� �������). ����� ������� ������
// ����� � ������������ ��� ���-�� �������� ���������.
void ASTBuilder::Run()
{
	if (astRoot)
		astRoot->Execute();
	else
		throw new Exception("���������� ��������� ���������� ������ �� ����������.");
		
}

// ����� ������ ������, ���������� ������ ��������� ��������.
void ASTBuilder::PrintList(List* tokens)
{
	char* stringToken[] = { "String", "SingleLineComment", "MultilineComment",
		"Digit", "Char", "Bracket", "ReservedWord", "Star", "CompOper", "LogicOper", "BiteOp",
		"Assignment", "SysFunction", "Separator", "UserType", "Space", "NewLine", "ReservedType",
		"Condition", "ApOp", "IncOrDec" };

	cout << "Type\t\tValue\t\tPriority\n";
	for (int i = 0; i < tokens->Count(); i++)
	{
		NewToken* token = (NewToken*)tokens->GetAccessor(i);

		cout << stringToken[token->type] << " ";
		cout << token->string << " ";
		cout << token->priority << " ";
		cout << token->value << " ";
		cout << token->lineIndex << endl;
	}
}

// ����������.
ASTBuilder::~ASTBuilder()
{
	if (astRoot) delete astRoot;

	astRoot = currentList = nullptr;
	currentScope = nullptr;
}

// ���������� ������ ��������, ������� ����� ����������� �������� Execute()
// � ������ �������� ���������, ��������� ���������, ����������, �������.
void ASTBuilder::Build()
{
	try
	{
		astRoot = currentList = new TList();
		currentScope = currentList->GetScope();

		ParseMultiLine(0, tokens->Count());
	}
	catch (Exception* exception)
	{
		if (strcmp(exception->GetMessage(),
			"ArgumentOutOfRange: �������� ��������� ��������� ��� ����������� ��������� ��������.") == 0)
		{
			delete exception;
			throw new Exception("UnexpectedEndOfFile: ����������� ����� �����.");
		}

		throw exception;
	}
}

// ���������� ������ ������ ��� openingBracketIndex ������.
// ���� ������ �� ������� ��� openingBracketIndex �� �������� �������,
// ����������� ���������� MissingBracket.
int ASTBuilder::ClosingBracketIndex(int openingBracketIndex)
{
	char* openingBracket = ((NewToken*)(tokens->GetAccessor(openingBracketIndex)))->string;
	char* closingBracket;

	if (strcmp(openingBracket, "(") == 0)
	{
		closingBracket = ")";
	}
	else if (strcmp(openingBracket, "{") == 0)
	{
		closingBracket = "}";
	}
	else if (strcmp(openingBracket, "[") == 0)
	{
		closingBracket = "]";
	}
	else
	{
		throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (openingBracketIndex))->lineIndex);
	}

	for (int count = 0, i = openingBracketIndex; i < tokens->Count(); i++)
	{
		char* next = ((NewToken*)(tokens->GetAccessor(i)))->string;

		if (strcmp(next, closingBracket) == 0)
		{
			count--;

			if (count == 0)
			{
				return i;
			}
		}

		if (strcmp(next, openingBracket) == 0)
		{
			count++;
		}
	}

	throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (openingBracketIndex))->lineIndex);
}

// ���������� true, ���� ����� �������� �������, � false � ��������� ������.
bool ASTBuilder::IsFraction(char* s)
{
	bool e = false;
	bool x = false;

	for (int i = 0; i < strlen(s); i++)
	{
		if (s[i] == '.') return true;
		if (s[i] == 'e' || s[i] == 'E') e = true;
		if (s[i] == 'x' || s[i] == 'X') x = true;
	}

	if (e && !x) return true;
	return false;
}

// ���������� true, ���� ������� �������� ����������, ����������
// �������������, ���������� �������� ��� ��������� �������,
// � false � ��������� ������.
bool ASTBuilder::IsAnExpression(int start, int end)
{
	for (int i = start; i <= end; i++)
	{
		NewToken* token = (NewToken*)tokens->GetAccessor(i);

		if (token->type == Automat::Bracket && strcmp(token->string, "[") == 0)
			i = ClosingBracketIndex(i);

		if (token->type == Automat::ApOp || token->type == Automat::CompOper || token->type == Automat::IncOrDec ||
			token->type == Automat::SysFunction || token->type == Automat::Assignment)
			return true;
	}

	return false;

}

// ������������ � double ����� ����� � ��������� �������� � ������� �����.
double ASTBuilder::StrToDouble(char* s)
{
	return IsFraction(s) ? atof(s) : strtol(s, nullptr, 0);
}

// ������� ������ �������� � ����������� ����������� � ������ tokens.
int ASTBuilder::FindOperationWithMinimalPriority(int startPosition, int endPosition)
{
	// 16 ������� ���������� �������� �� 1 �� 16.
	int min = 17, result = -1;

	for (int i = startPosition; i <= endPosition; i++)
	{
		NewToken* l = (NewToken*)tokens->GetAccessor(i);

		if (isOperator(l->type))
		{
			// ���������� ������.
			if (!strcmp(l->string, "[") && min == 17)
			{
				int f = ClosingBracketIndex(i);

				if (f == endPosition)
					return i;
				else
					i = f;
			}

			// ������������.
			if (l->priority < min && l->priority == 2)
			{
				min = l->priority;
				result = i;
			}

			// ������ ��������.
			if (l->priority <= min && l->priority != 2)
			{
				min = l->priority;
				result = i;
			}
		}

		// �������� �� ����������� ������� ������.
		if (l->type == Automat::Bracket && strcmp(l->string, "(") == 0)
				i = ClosingBracketIndex(i);
	}

	if (result == -1) 
		throw new Exception("InvalidOperation: ������������ ��������.", ((NewToken*)tokens->GetAccessor (startPosition))->lineIndex);


	return result;
}

// ����������� �������� ���������� ��� �������� � ������ tokens
// �� 1 �� 16, ��� 16 - ��������� ���������.
void ASTBuilder::Prioritize(List* tokenList)
{
	NewToken* current;
	NewToken* prev;

	for (int i = 0; i < tokenList->Count(); i++)
	{
		current = ((NewToken*)(tokenList->GetAccessor(i)));
		prev = NULL;
		if (i > 0)
		{
			prev = ((NewToken*)(tokenList->GetAccessor(i - 1)));
		}

		if (isOperator(current->type))
		{
			string s = "";
			s += current->string;

			// �������.
			if (s == ",")
			{
				current->priority = 1;
				continue;
			}

			// ������������.
			if (s == "=")
			{
				current->priority = 2;
				continue;
			}

			// ��������� �������� �������� (�����������).
			if ((s == "?") || (s == ":"))
			{
				current->priority = 3;
				continue;
			}

			// ���������� ���.
			if (s == "||")
			{
				current->priority = 4;
				continue;
			}

			// ���������� �.
			if (s == "&&")
			{
				current->priority = 5;
				continue;
			}

			// ��������� ���.
			if (s == "|")
			{
				current->priority = 6;
				continue;
			}

			// ��������� ����������� ���.
			if (s == "^")
			{
				current->priority = 7;
				continue;
			}

			// ������������� ��������� (�����������), ������ ������ (�����������),
			// ������� ���� � �����, ���������� ��������� � ���������.
			if ((s == "*") || (s == "&") || (s == "+") || (s == "-") || (s == "++") || (s == "--"))
				current->priority = 15;

			// �������� ���� � �����.
			if (((s == "+") || (s == "-")) && ((prev->type == Automat::Token::Digit) || prev->type == Automat::Token::UserType
				|| prev->type == Automat::Token::IncOrDec || !strcmp(prev->string, ")") || !strcmp(prev->string, "]")))
			{
				current->priority = 12;
				continue;
			}

			// ��������� �.
			if ((s == "&") && (prev->type == Automat::Token::UserType))
			{
				current->priority = 8;
				continue;
			}

			// ��������� � �����������.
			if ((s == "==") || (s == "!="))
			{
				current->priority = 9;
				continue;
			}

			// ������, ������, ������ ��� �����, ������ ��� �����.
			if ((s == "<") || (s == ">") || (s == "<=") || (s == ">="))
			{
				current->priority = 10;
				continue;
			}
				
			// ���������, �������, ��������� ������� �� �������.
			if (((s == "*") || (s == "/") || (s == "%")) && ((prev->type == Automat::Token::Digit)
				|| (prev->type == Automat::Token::UserType) || !strcmp(prev->string, "]") || !strcmp(prev->string, ")")))
			{
				current->priority = 13;
				continue;
			}
			
			// ���������� ��������� � ���������, ����� �������� �� ��������� (�����������), ������ �������� �������.
			if (((s == "++") || (s == "--") || (s == "->") || (s == "[")) && (prev->type == Automat::Token::UserType) || (s == "]"))
				current->priority = 16;

			// ��������� �������
			if (current->type == Automat::SysFunction)
				current->priority = 15;
		}
	}
}

// ����� ������� ������ token � ������ tokens � ������� startPosition �� endPosition.
// ���� ����� �� ������, ������������� ���������� TokenNotFound.
int ASTBuilder::FindToken(int startPosition, int endPosition , char* token)
{
	for (int i = startPosition; i <= endPosition && i < tokens->Count(); i++)
	{
		if (!strcmp(((NewToken*)tokens->GetAccessor(i))->string, token))
			return i;
	}
	
	throw new Exception("TokenNotFound: ����� �� ������.", ((NewToken*)tokens->GetAccessor (startPosition))->lineIndex);
}

// ���������� true, ���� token �������� ��������� � false � ��������� ������.
bool ASTBuilder::isOperator(Automat::Token token)
{
	switch (token)
	{
		case Automat::Token::ApOp:
		case Automat::Token::Assignment:
		case Automat::Token::BiteOp:
		case Automat::Token::CompOper:
		case Automat::Token::Condition:
		case Automat::Token::IncOrDec:
		case Automat::Token::LogicOper:
		case Automat::Token::ReservedWord:
		case Automat::Token::Separator:
		case Automat::Token::Star:
		case Automat::Token::SysFunction:
			return true;
		default:
			return false;
	}
}

// ����������� ������� � ����� ������ NewToken � ������������
// � ���������� �������� value ��� �����.
List* ASTBuilder::OldTokenToNewToken(List* old)
{
	List* newTokenList = new List(sizeof(NewToken));
	int countLines = 1;

	for (int i = 0; i < old->Count(); i++)
	{
		Automat::ParseResult* oldToken = (Automat::ParseResult*)old->GetAccessor(i);

		// ����� ��������, ��������� ����� � ������������.
		if (oldToken->token == Automat::Token::Space ||
			oldToken->token == Automat::Token::NewLine ||
			oldToken->token == Automat::Token::SingleLineComment ||
			oldToken->token == Automat::Token::MultilineComment)
		{
			if (oldToken->token == Automat::NewLine) countLines++;
			continue;
		}

		if (oldToken->token == Automat::ReservedWord)
		{
			if (strcmp(oldToken->string, "true") == 0)
			{
				oldToken->token = Automat::Digit;
				oldToken->string = "1";
			}
			if (strcmp(oldToken->string, "false") == 0)
			{
				oldToken->token = Automat::Digit;
				oldToken->string = "0";
			}
		}

		// ���� �����, �� ��������� ��� ��������, ����� 0.
		double value = 0;

		if (oldToken->token == Automat::Token::Digit)
		{
			value = StrToDouble(oldToken->string);
		}

		if (oldToken->token == Automat::Token::SysFunction)
		{
			// ��� ��������� �������, ������������ �������� (����, ����� input � output),
			// �������� value = 1.
			if (strcmp(oldToken->string, "output") && strcmp(oldToken->string, "input"))
			{
				value = 1;
			}
		}

		
		// ���������� ��� �������� ������������� ����� ������� Prioritize(List* tokenList).
		NewToken* newToken = new NewToken(oldToken->token, oldToken->string, value, 0, countLines);
		newTokenList->Add(newToken);
	}

	return newTokenList;
}

// ���� ������������ ����� � ������, �� ����� ���� ��� ���������� ��� ����������.
// ���������� (TConst : TNode) ��� (TVariable : TNode), ��� ���������� � ������ TList* root.
// ���� ����� �� �������� ���������� ��� ����������, ���������� InvalidOperation.
// ���� ���������� �� �������, ���������� VariableNameNotFound.
TNode* ASTBuilder::ParseSingleTokenLine(int index)
{
	NewToken* firstElement = (NewToken*)tokens->GetAccessor(index);

	// ���������.
	if (firstElement->type == Automat::Digit || firstElement->type == Automat::Char)
	{
		if (firstElement->type == Automat::Digit)
		{
			TValue* value = new TValue(firstElement->value, TypeList::Instance().GetTypeIndex("double"));
			return new TConst(value);
		}
		else
		{
			char symbol = firstElement->string[1];
			TValue* value = new TValue(symbol, TypeList::Instance().GetTypeIndex("char"));
			return new TConst(value);
		}
	}

	// ���������� ��� ���������.
	if (firstElement->type == Automat::UserType)
	{
		TValueKeeper* var = currentScope->Find(firstElement->string);

		// ���������� �� �������.
		if (var == nullptr)
			throw new Exception("VariableNameNotFound: ������������� ���������� ��� ���������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
		
		// ���������� ������ ������ �� ������ ��������.
		if (var->IsReference())
			throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

		if (var->IsConst())
		{
			// ���������.
			return new TConst(var->GetValue());
		}
		else
		{
			// ����������.
			return new TVariable(var->GetValue());
		}
	}

	throw new Exception("InvalidCharacter: ������������ ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
}

// ���������� � ������������� ��������� const.
// �������� ��������� ������ ���� ���� ������� � �������� ��� ����������:
// ect.: const int a = 5;
void ASTBuilder::ParseConstInitialization(int& index)
{
	// index	index + 1	index + 2	index + 3	index + 4	index + 5 = semicolon
	// const	type		name		=			value		;

	// index	index + 1	index + 2	index + 3	index + 4	index + 5	index + 6 = semicolon
	// const	type		name		=			- (+)		value		;

	int semicolon = FindToken(index, tokens->Count(), ";");
	int lenght = semicolon - index;

	if (lenght != 5 && lenght != 6)
		throw new Exception("ConstInitializationError: ������ ������������� ���������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	NewToken* type = (NewToken*)tokens->GetAccessor(++index);	// ���.

	if (TypeList::Instance().GetTypeIndex(type->string) == -1)
		throw new Exception("UnknownType: ���������������� ��� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	NewToken* name = (NewToken*)tokens->GetAccessor(++index);	// ��� ���������.

	if (name->type != Automat::UserType)
		throw new Exception("ConstInitializationError: ������ ������������� ���������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	if (currentScope->FindInCurrentScope(name->string) != nullptr)
		throw new Exception("RedefinitionVariable: ������������ ��������������� ����������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	NewToken* assignment = (NewToken*)tokens->GetAccessor(++index);

	if (assignment->type != Automat::Assignment)
		throw new Exception("UninitializedConstant: �������������������� ���������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	int sign = 1; // ����.

	if (lenght == 6)
	{
		NewToken* signToken = (NewToken*)tokens->GetAccessor(++index);

		if (strcmp(signToken->string, "-") == 0)
			sign = -1;
		else if (strcmp(signToken->string, "+") == 0)
			sign = 1;
		else
			throw new Exception("ConstInitializationError: ������ ������������� ���������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
	}

	NewToken* value = (NewToken*)tokens->GetAccessor(++index);	// ��������.

	if (value->type == Automat::Digit)
	{
		currentScope->Push(new TValueKeeper(name->string, new TValue(sign * (value->value), TypeList::Instance().GetTypeIndex(type->string), 0), true));
		return;
	}

	throw new Exception("ConstInitializationError: ������ ������������� ���������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
}

// ���������� (� �������������) ���������� ��� ���������� ������������ �������.
// �������������������� ������������ ���������� � ��������� �������� ������������� �������� 0.
// ���� ���������� ���������������� ��������� ����, ��������������� TBinaryOperation* ��� �������� =
// ����������� � currentList ��� ������������ ����������.
void ASTBuilder::ParseInitialization(int& index)
{
	// 1. ����������
	// index	index + 1	index + 2 = semicolon
	// type		name		;

	// 2. ���������� � ������������� ��������
	// index	index + 1	index + 2	index + 3	index + n = semicolon
	// type		name		=			value		;

	// 3. ������
	// index	index + 1	index + 2	index + 3	index + 4	index + 5 = semicolon
	// type		name		[			size		]			;

	int semicolon = FindToken(index, tokens->Count(), ";");

	if (semicolon - index < 2)
		throw new Exception("VariableNameIsNotSpecified: �� ������� ��� ����������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	// ��� ����������.
	NewToken* type = (NewToken*)tokens->GetAccessor(index);

	if (TypeList::Instance().GetTypeIndex(type->string) == -1)
		throw new Exception("UnknownType: ���������������� ��� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	// ��� ����������.
	NewToken* name = (NewToken*)tokens->GetAccessor(++index);

	if (name->type != Automat::UserType)
		throw new Exception("InvalidVariableName: ������������ ��� ����������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	if (currentScope->FindInCurrentScope(name->string) != nullptr)
		throw new Exception("RedefinitionVariable: ������������ ��������������� ����������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	if (semicolon - index == 1)
	{
		// 1. ���������� ��� ������������: ��������� � ��� �� ��������� �� ��������� 0. +
		currentScope->Push(new TValueKeeper(name->string, new TValue(0, TypeList::Instance().GetTypeIndex(type->string), 0)));
		index = semicolon;
		return;
	}

	NewToken* bracketOrAssignment = (NewToken*)tokens->GetAccessor(++index);

	// 2. ���������� � �������������.
	if (bracketOrAssignment->type == Automat::Assignment)
	{
		TValue* tValue = new TValue(0, TypeList::Instance().GetTypeIndex(type->string), 0);

		// ��������� ���������� � ��� �� ��������� �� ��������� 0.
		currentScope->Push(new TValueKeeper(name->string, tValue));

		// ������������.
		NewToken* assignment = (NewToken*)tokens->GetAccessor(index);
		if (assignment->type != Automat::Assignment)
			throw new Exception("InvalidOperation: ������������ ��������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

		// ���������� ��� ������.
		if (!strcmp(type->string, "char"))
		{
			char symbol = ((NewToken*)tokens->GetAccessor(++index))->string[1];

			currentList->Push(new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex("="), new TVariable(currentScope->FindInCurrentScope(name->string)->GetValue()), new TConst(new TValue(symbol, TypeList::Instance().GetTypeIndex("char")))));
			return;
		}

		// ��������� ��������.
		TNode* value = ParseLine(++index, semicolon - 1); 

		currentList->Push(new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex("="), new TVariable(currentScope->FindInCurrentScope(name->string)->GetValue()), value));

		index = semicolon;
		return;
	}

	// 3. ������.
	if (bracketOrAssignment->type == Automat::Bracket && semicolon - index == 3)
	{
		// ������.
		NewToken* size = (NewToken*)tokens->GetAccessor(++index);
		int integerSize = 0;

		// ����� ������������� �������� ��������.
		if (size->type == Automat::Digit)
		{
			if (size->value <= 0 || this->IsFraction(size->string))
				throw new Exception("ArraySizeIsNotAnInteger: ������ ������� ������ ���� ����� ������������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

			integerSize = (int)size->value;
		}
		// ���������, ����������� �����.
		else if (size->type == Automat::UserType)
		{
			TValueKeeper* constSize = (currentScope->FindInCurrentScope(size->string));

			if (constSize == nullptr)
			{
				constSize = (currentScope->Find(size->string));

				if (constSize == nullptr)
				{
					throw new Exception("VariableNameNotFound: ������������� ���������� ��� ���������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
				}
				else
				{
					if (constSize->IsConst())
					{
						if (constSize->GetValue()->GetType() == TypeList::Instance().GetTypeIndex("int"))
						{
							integerSize = (int)constSize->GetValue()->GetValue();

							if (integerSize <= 0)
								throw new Exception("ArraySizeIsNotAnInteger: ������ ������� ������ ���� ����� ������������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
						}
						else
							throw new Exception("ArraySizeIsNotAnInteger: ������ ������� ������ ���� ����� ������������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
					}
					else
						throw new Exception("ArraySizeIsNotAConstant: ������ ������� ������ ���� ����� ����������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
				}
			}
			else
			{
				if (constSize->IsConst())
				{
					if (constSize->GetValue()->GetType() == TypeList::Instance().GetTypeIndex("int"))
					{
						integerSize = (int)constSize->GetValue()->GetValue();

						if (integerSize <= 0)
							throw new Exception("ArraySizeIsNotAnInteger: ������ ������� ������ ���� ����� ������������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
					}
					else
						throw new Exception("ArraySizeIsNotAnInteger: ������ ������� ������ ���� ����� ������������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
				}
				else
					throw new Exception("ArraySizeIsNotAConstant: ������ ������� ������ ���� ����� ����������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
			}
		}

		// ����������� ������.
		index++;
		if (index != ClosingBracketIndex(index - 2))
			throw new Exception("ArrayInitializationError: ������ ���������� �������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

		currentScope->Push(new TValueKeeper(name->string, new TValue(0, TypeList::Instance().GetTypeIndex(type->string), integerSize)));
		index = semicolon;
		return;
	}

	throw new Exception("InitializationError: ������ ���������� ��� �������������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
}

// ����������� if-else.
TNode* ASTBuilder::ParseIfElse(int& index)
{
	// if (condition)
	// {
	//		body;
	// }
	// else *
	// { *
	//		body; *
	// } *
	// * - optional

	// 1. ������� if.
	NewToken* openingParenthesis = (NewToken*)tokens->GetAccessor(++index);

	if (strcmp(openingParenthesis->string, "(") != 0)
		throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	int closingParenthesis = ClosingBracketIndex(index);

	TNode* condition = ParseLine(index, closingParenthesis);
	index = closingParenthesis;

	// 2. ���� then.
	NewToken* openingBrace = (NewToken*)tokens->GetAccessor(++index);
	if (strcmp(openingBrace->string, "{") != 0)
		throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

	int closingBrace = ClosingBracketIndex(index);

	// ������� TList ��� ���� then � ��������������� �������� ��������� Scope.
	// ������� currentList ��������� � tempList.
	// ������ currentList � currentScope �� TList � Scope ���� �����.
	TList* thenBranch = new TList(currentList);
	TList* tempList = currentList;
	currentList = thenBranch;
	currentScope = thenBranch->GetScope();

	// ������ ���� then.
	ParseMultiLine(index + 1, closingBrace - 1);

	// ��������������� currentList � ������� ��������� currentScope.
	currentList = tempList;
	currentScope = currentList->GetScope();

	index = closingBrace;

	// 3. ���� else.

	// ���� �� ������ ��� ����� ���� then. @ �� ����, ��������, ����� ����� ������.
	if (index == tokens->Count() - 1)
	{
		return new TIf(condition, thenBranch, nullptr);
	}

	int indexBeforeElseBranch = index;

	NewToken* elseToken = (NewToken*)tokens->GetAccessor(++index);
	TList* elseBranch = nullptr;

	// ���� ���� else.
	if (elseToken && strcmp(elseToken->string, "else") == 0)
	{
		NewToken* openingBrace = (NewToken*)tokens->GetAccessor(++index);
		if (openingBrace->type != Automat::Bracket)
			throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);

		int closingBrace = ClosingBracketIndex(index);

		// ���������� then.
		elseBranch = new TList(currentList);
		tempList = currentList;
		currentList = elseBranch;
		currentScope = elseBranch->GetScope();

		ParseMultiLine(index + 1, closingBrace - 1);

		currentList = tempList;
		currentScope = currentList->GetScope();

		index = closingBrace;
	}

	if (elseBranch == nullptr) index = indexBeforeElseBranch;

	return new TIf(condition, thenBranch, elseBranch);
}

// ���� while.
TNode* ASTBuilder::ParseWhile(int& index)
{
	// while (condition)
	// {
	//		body;
	// }

	// 1. �������.
	NewToken* openingParenthesis = (NewToken*)tokens->GetAccessor(++index);

	if (strcmp(openingParenthesis->string, "(") != 0)
		throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
	int closingParenthesis = ClosingBracketIndex(index);

	TNode* condition = ParseLine(index, closingParenthesis);

	index = closingParenthesis;

	// 2. ���� �����.
	NewToken* openingBrace = (NewToken*)tokens->GetAccessor(++index);
	if (strcmp(openingBrace->string, "{") != 0)
		throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
	int closingBrace = ClosingBracketIndex(index);

	TList* body = new TList(currentList);
	TList* tempList = currentList;
	currentList = body;
	currentScope = body->GetScope();

	ParseMultiLine(index + 1, closingBrace - 1);

	index = closingBrace;

	currentList = tempList;
	currentScope = tempList->GetScope();

	return new TWhile(condition, body);
}

// ����� ���������� � ��������� � ��������� ��������.
TNode* ASTBuilder::ParseVariableName(int& index, TNodeType& type)
{
	// �������� ����������.
	NewToken* variableName = (NewToken*)tokens->GetAccessor(index++);

	TValueKeeper* keeper = currentScope->Find(variableName->string);
	if (keeper == nullptr)
		throw new Exception("VariableNameNotFound: ������������� ���������� ��� ���������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);


	// ���������� ������ ������ �� ������ ��������.
	// ��� ������� � ������� �������� ���������� �������� ������� �������� � ���������� �������.
	if (keeper->IsReference())
	{
		//  ���������� ������.
		NewToken* brackets = (NewToken*)tokens->GetAccessor(index);

		if (brackets->type == Automat::Bracket && strcmp(brackets->string, "[") == 0)
		{
			int closeBracket = ClosingBracketIndex(index);

			// ������ ��������.
			TNode* elemIndex = ParseLine(index + 1, closeBracket - 1);

			index = closeBracket + 1;

			type = TNodeType::Array;
			return new TArray(keeper->GetValue(), elemIndex);
		}
		else
		{
			// ��� ������ ����� ����������, �������� ������ �� ������.
			throw new Exception("IndexIsNotSpecified: �� ������ ������ �������� �������.", ((NewToken*)tokens->GetAccessor (index))->lineIndex);
		}
	}
	else
	{
		// ���������. 
		if (keeper->IsConst())
		{
			type = TNodeType::Const;
			return new TConst(keeper->GetValue());
		}
		// ����������.
		else
		{
			type = TNodeType::Variable;
			return new TVariable(keeper->GetValue());
		}
	}
}

// ��� �������� ������������ ������ (TNode*)
// 1. ������������ ����� � ������ (TConst* ��� TVariable*) ��� ������� ������� (TVariable*)
// 2. Line �� �������� ����������, ���������� ��������������, ���������� ��������,
//    �������� ������������ ��� ��������� �������.
// 3. �������������� �������� (TUnaryOperation* ��� TBinaryOperation*)
// 4. �������� ��������� (TBinaryOperation*)
// 5. ��������� � ���������
// 6. ��������� �������, ������������ �������� (TUnaryOperation*)
// 7. ������������
// 8. ���������� ��������
TNode* ASTBuilder::ParseLine(int start, int end)
{
	NewToken* token = (NewToken*)tokens->GetAccessor(start);

	// ������� ����������� ������.
	NewToken* i = token;
	while (i->type == Automat::Token::Bracket && ClosingBracketIndex(start) == end)
	{
		start++;
		end--;

		i = (NewToken*)tokens->GetAccessor(start);
	}

	// 1. ������������ ����� � ������.
	if (end == start)
	{
		return ParseSingleTokenLine(start);
	}

	// 2. Line �� �������� ����������, ���������� ��������������, ���������� ��������,
	// �������� ������������ ��� ��������� �������.
	if (IsAnExpression(start, end) == false)
	{
		TNodeType type; // �� �����, ������ ������. @
		return ParseVariableName(start, type);
	}

	// ����� �������� � ����������� �����������.
	int pos = FindOperationWithMinimalPriority(start, end);
	NewToken* element = (NewToken*)tokens->GetAccessor(pos);

	// 3. �������������� ��������.
	if (element->type == Automat::ApOp)
	{
		// ������� ��������.
		if (element->priority == 15)
		{
			return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(element->string), ParseLine(pos + 1, end));
		}

		return new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex(element->string), ParseLine(start, pos - 1), ParseLine(pos + 1, end));
	}

	// 4. ��������� ���������.
	if (element->type == Automat::CompOper)
	{
		return new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex(element->string), ParseLine(start, pos - 1), ParseLine(pos + 1, end));
	}

	// 5. ��������� � ���������.
	if (element->type == Automat::IncOrDec)
	{
		// ���������.
		if (!strcmp(element->string, "++"))
		{
			if (((NewToken*)tokens->GetAccessor(pos + 1))->type == Automat::UserType)
			{
				// ����������.
				return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex("++�"), ParseLine(pos + 1, end));
			}

			// ����������.
			return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex("�++"), ParseLine(start, pos - 1));
		}
		if (!strcmp(element->string, "--"))
		{
			if (((NewToken*)tokens->GetAccessor(pos + 1))->type == Automat::UserType)
			{
				// ����������.
				return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex("--�"), ParseLine(pos + 1, end));
			}

			// ����������.
			return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex("�--"), ParseLine(start, pos - 1));
		}

		throw new Exception("UnknownOperation: ���������������� ��������.", ((NewToken*)tokens->GetAccessor (start))->lineIndex);
	}

	// 6. ��������� �������, ������������ ��������.
	if (element->type == Automat::SysFunction && element->value)
	{
		NewToken* bracket = (NewToken*)tokens->GetAccessor(++pos);

		if (bracket->type != Automat::Bracket)
			throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (pos))->lineIndex);

		int closeBracket = ClosingBracketIndex(pos);

		// �� ������� ��������� �������.
		if (closeBracket - pos == 1)
			throw new Exception("MissingArguments: ��������� ��������� ��� ������ �������.", ((NewToken*)tokens->GetAccessor (pos))->lineIndex);

		if (UnaryOperationList::Instance().IsOperation(element->string))
		{
			TNode* argument = ParseLine(pos, closeBracket);
			TUnaryOperation* unaryOperation = new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(element->string), argument);
			return unaryOperation;
		}

		if (BinaryOperationList::Instance().IsOperation(element->string))
		{
			int separatorIndex = -1;

			for (int i = pos; i < closeBracket; i++)
			{
				NewToken* separator = (NewToken*)tokens->GetAccessor(i);

				if (separator->type == Automat::Separator && strcmp(separator->string, ",") == 0)
				{
					separatorIndex = i;
					break;
				}
			}

			// �� ������� ��� ��������� �������.
			if (separatorIndex == -1)
				throw new Exception("MissingArguments: ��������� ��������� ��� ������ �������.", ((NewToken*)tokens->GetAccessor (pos))->lineIndex);

			TNode* first = ParseLine(pos + 1, separatorIndex - 1);
			TNode* second = ParseLine(separatorIndex + 1, closeBracket - 1);

			TBinaryOperation* binaryOperation = new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex(element->string), first, second);
			return binaryOperation;
		}

		throw new Exception("SystemFunctionIsNotSupported: ���������������� ��������� �������.", ((NewToken*)tokens->GetAccessor (start))->lineIndex);
	}

	// 7. ������������
	// a = <expression>;
	// a [<index>] = <expression>;
	if (element->type == Automat::Assignment)
	{
		TNodeType resultType;
		TNode* left = ParseVariableName(start, resultType);
		TNode* right = ParseLine(pos + 1, end);

		// ������������ �������� ��������� ���������.
		if (resultType == TNodeType::Const)
			throw new Exception("ConstantReinitialization: ��������� �������� ��������� ����������.", ((NewToken*)tokens->GetAccessor (start))->lineIndex);

		return new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex("="), left, right);
	}

	//8. ���������� ��������
	if (element->type == Automat::LogicOper)
	{
		if (strcmp(element->string, "!"))
		{
			return new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex(element->string), ParseLine(start, pos - 1), ParseLine(pos + 1, end));
		}
		else
		{
			NewToken* bracket = (NewToken*)tokens->GetAccessor(++pos);

			if (bracket->type != Automat::Bracket)
				throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (pos))->lineIndex);

			int closeBracket = ClosingBracketIndex(pos);
			return new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(element->string), ParseLine(pos + 1, closeBracket - 1));
		}
	}

	throw new Exception("InvalidOperation: ������������ ��������.", ((NewToken*)tokens->GetAccessor (start))->lineIndex);
}

// ��� �������� ��������� � ���� ����������� if � ������.
// �� ���������� ��������, ��� TNode* ��������� � currentList.
// 1. ���������� (� �������������) ��������, ����������, ��������.
// 2. ����� if (TIfElse*) � while (TWhile)
// 3. ������� input() � output().
// 4. ��������� � ������������ �������� ���������� � ��������� ��������.
void ASTBuilder::ParseMultiLine(int start, int end)
{
	for (int i = start; i < end && i < tokens->Count(); i++)
	{
		NewToken* token = (NewToken*)tokens->GetAccessor(i);

		// ������� ����� � �������.
		if (token->type == Automat::Separator)
			continue;

		// 1. ���������� ���������.
		if (strcmp(token->string, "const") == 0)
		{
			// const int a = <expression>;
			ParseConstInitialization(i);
			continue;
		}

		// 2. ���������� ���������� � ��������.
		// ��� ��� = <expression>;
		// ��� ���[������];
		if (token->type == Automat::Token::ReservedType)
		{
			ParseInitialization(i);
			continue;
		}

		// 3. ����������� if � while.
		if (token->type == Automat::Token::Condition)
		{
			if (strcmp(token->string, "if") == 0)
			{
				currentList->Push(ParseIfElse(i));
				continue;
			}
			else if (strcmp(token->string, "while") == 0)
			{
				currentList->Push(ParseWhile(i));
				continue;
			}
			else
			{
				throw new Exception("InvalidOperation: ������������ ��������.", ((NewToken*)tokens->GetAccessor (i))->lineIndex);
			}
		}

		// 4. ��������� ������� c ����� ���������� input � output, �� ������������ ��������.
		if (token->type == Automat::Token::SysFunction && token->value == 0)
		{
			// �������� ������� � ������� �������.
			NewToken* bracket = (NewToken*)tokens->GetAccessor(++i);

			if (bracket->type != Automat::Token::Bracket)
				throw new Exception("MissingBracket: ��������� ������.", ((NewToken*)tokens->GetAccessor (i))->lineIndex);

			int closeBracketIndex = ClosingBracketIndex(i);

			// ��������� �� �������.
			if (closeBracketIndex - i == 1)
				throw new Exception("MissingArguments: ��������� ��������� ��� ������ �������.", ((NewToken*)tokens->GetAccessor (i))->lineIndex);

			i++;

			NewToken* digitOrChar = (NewToken*)tokens->GetAccessor(i);

			if (digitOrChar->type == Automat::UserType)
			{
				TNode* argument = ParseLine(i, closeBracketIndex - 1);

				// �����.
				if (!strcmp(token->string, "output"))
				{
					currentList->Push(new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(token->string), argument));
				}
				// ����.
				else
				{
					currentList->Push(new TBinaryOperation(BinaryOperationList::Instance().GetOperationIndex("="), argument, new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(token->string), argument)));
				}
			}
			else if (digitOrChar->type == Automat::String)
			{
				// �������� ����������� �������.
				int stringLenght = strlen(digitOrChar->string) - 2 + 1;

				TValue* tValueString = new TValue(0, TypeList::Instance().GetTypeIndex("char"), stringLenght);
				
				double* doubleString = new double[stringLenght];
				for (int i = 0, j = 1; i < stringLenght - 1; i++, j++)
				{
					doubleString[i] = digitOrChar->string[j];
				}
				doubleString[stringLenght] = '\0';

				tValueString->SetPointer(doubleString);

				// ����� ���������� �� ������� ���� �����������, ��� ��� ��� ��� �� ���������. @
				currentScope->Push(new TValueKeeper("@string", tValueString));
				
				currentList->Push(new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(token->string), new TVariable(tValueString)));
			}
			else
			{
				TNode* argument = ParseLine(i, closeBracketIndex - 1);
				currentList->Push(new TUnaryOperation(UnaryOperationList::Instance().GetOperationIndex(token->string), argument));
			}

			i = closeBracketIndex;

			continue;
		}

		// 5. ��������� � ���������� � ��������
		// a = <expression>;
		// a [i] = <expression>; 
		int semicolon = FindToken(i, tokens->Count(), ";");
		currentList->Push(ParseLine(i, semicolon - 1));
		i = semicolon;
	}
}