// 
// Pizza.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "Pizzeria/Pizza.h"


namespace Pizzeria {


Pizza::Pizza():
	_basePrice(),
	_details(),
	_name(),
	_toppings()
{
}


Pizza::Pizza(Poco::UInt32 basePrice, const PizzaDetails& details, const std::string& name, const std::vector<ExtTopping>& toppings):
	_basePrice(basePrice),
	_details(details),
	_name(name),
	_toppings(toppings)
{
}


Pizza::~Pizza()
{
}


} // Pizzeria


