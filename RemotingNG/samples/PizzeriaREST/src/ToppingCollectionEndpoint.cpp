//
// ToppingCollectionEndpoint.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Pizzeria/ToppingCollectionEndpoint.h"


namespace Pizzeria { 


ToppingCollectionEndpoint::ToppingCollectionEndpoint(const std::vector<ExtTopping>& toppings):
	_toppings(toppings)
{
}


std::vector<ExtTopping> ToppingCollectionEndpoint::get() const
{
	return _toppings;
}


} // namespace Pizzeria
