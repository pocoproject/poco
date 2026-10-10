//
// PizzaCollectionEndpoint.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Pizzeria/PizzaCollectionEndpoint.h"


namespace Pizzeria {


PizzaCollectionEndpoint::PizzaCollectionEndpoint(const std::vector<Pizza>& pizzas):
	_pizzas(pizzas)
{
}


std::vector<Pizza> PizzaCollectionEndpoint::get() const
{
	return _pizzas;
}


} // namespace Pizzeria
