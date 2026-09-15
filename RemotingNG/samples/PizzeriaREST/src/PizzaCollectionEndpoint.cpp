//
// PizzaCollectionEndpoint.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
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
