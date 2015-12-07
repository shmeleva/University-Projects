#include "stdafx.h"
#include "Defaults.h"
#include "ASTBuilder.h"
#include "Exception.h"

#include <ctime>
#include <iostream>

#define GAME1 "bullsAndCows.cpp"
#define GAME2 "towelOfHanoi.cpp"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(nullptr));

	try
	{
		ASTBuilder* builder = new ASTBuilder(GAME1);
		builder->Build();
		builder->Run();
	}
	catch (Exception* exception)
	{
		setlocale(LC_ALL, "Russian");
		cout << exception->GetMessage() << " Line: " << exception->GetLine() << "." << endl;
	}

	return 0;
}