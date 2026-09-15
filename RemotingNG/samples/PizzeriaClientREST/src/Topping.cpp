//
// Topping.cpp
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Pizzeria/Topping.h"
#include "Poco/NumberFormatter.h"


namespace Pizzeria {


Topping::Topping():
	_name(),
	_price(0)
{
}


Topping::Topping(const std::string& name, Cents price):
	_name(name),
	_price(price)
{
}


Topping::~Topping()
{
}


std::string Topping::toString() const
{
	std::string tmp;
	tmp.append("[");
	tmp.append(getName());
	tmp.append(",");
	tmp.append(Poco::NumberFormatter::format(getPrice()));
	tmp.append("]");
	return tmp;
}


ExtTopping::ExtTopping(): Topping()
	
{
}


ExtTopping::ExtTopping(const std::string& name, Cents price):
	Topping(name, price)
{
}


ExtTopping::~ExtTopping()
{
}


} // namespace Pizzeria
