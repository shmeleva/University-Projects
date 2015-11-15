#pragma once

#include <winsock2.h>
#include <iostream>
#include <sstream>

class Exception
{
public:
	virtual std::string GetExceptionMessage() = 0;
};

class WebException : public Exception
{	
	std::string function;
	int error;

public:
	WebException(std::string function, int error);
	~WebException();

	int GetError();
	std::string GetExceptionMessage();
};

class ArgumentException : public Exception
{
	std::string message;

public:
	ArgumentException(std::string message);
	~ArgumentException();

	std::string GetExceptionMessage();
};

