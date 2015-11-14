#include "stdafx.h"
#include "WebException.h"


WebException::WebException(std::string function, int error)
{
	this->function = function;
	this->error = error;
}

WebException::~WebException()
{
	error = 0;
}

int WebException::GetError()
{
	return error;
}

std::string WebException::GetExceptionMessage()
{
	return (function.empty() ? "Error " : function + " failed with error ") + std::to_string(error) + ".";
}

ArgumentException::ArgumentException(std::string message)
{
	this->message = message;
}

ArgumentException::~ArgumentException()
{
}

std::string ArgumentException::GetExceptionMessage()
{
	return message;
}