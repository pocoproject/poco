// 
// Exception.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "Pizzeria/Exception.h"


namespace Pizzeria {


Exception::Exception():
	_type(),
	_message(),
	_code()
{
}


Exception::Exception(const std::string& type, const std::string& message, int code):
	_type(type),
	_message(message),
	_code(code)
{
}


Exception::~Exception()
{
}


} // Pizzeria


