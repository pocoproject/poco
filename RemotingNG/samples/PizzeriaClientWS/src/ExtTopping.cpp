// 
// ExtTopping.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "Pizzeria/ExtTopping.h"


namespace Pizzeria {


ExtTopping::ExtTopping():
	_addInfo()
{
}


ExtTopping::ExtTopping(const std::string& name, Poco::UInt32 price, const std::string& addInfo):
	Topping(name, price),
	_addInfo(addInfo)
{
}


ExtTopping::~ExtTopping()
{
}


} // Pizzeria


