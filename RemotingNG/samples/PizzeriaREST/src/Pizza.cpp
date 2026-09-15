//
// Pizza.cpp
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Pizzeria/Pizza.h"
#include "Poco/NumberFormatter.h"


namespace Pizzeria {


Pizza::Pizza(const std::string& name, Cents basePrice):
	_name(name),
	_toppings(),
	_basePrice(basePrice)
{
	_details.calories = 350;
	_details.percentFat = 35;
	_details.carboHydrates = 12;
}


Pizza::~Pizza()
{
}


Cents Pizza::price() const
{
	Cents cost = _basePrice;
	for (ExtToppings::const_iterator it = _toppings.begin(); it != _toppings.end(); ++it)
	{
		cost += it->getPrice();
	}
	return cost;
}


void Pizza::remove(const std::string& toppingId)
{
	for (ExtToppings::iterator it = _toppings.begin(); it != _toppings.end(); ++it)
	{
		if (it->getName() == toppingId)
		{
			_toppings.erase(it);
			return;
		}
	}
}


std::string Pizza::toString() const
{
	std::string tmp(_name);
	tmp.append("(");
	for (ExtToppings::const_iterator it = _toppings.begin(); it != _toppings.end(); ++it)
	{
		tmp.append(it->toString());
	}
	tmp.append(" Price: ");
	tmp.append(Poco::NumberFormatter::format(price()/100.0, 2));
	tmp.append(")");
	return tmp;
}


} // namespace Pizzeria
