//
// PizzaCollectionEndpoint.h
//
// Definition of the PizzaCollectionEndpoint class.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PizzaCollectionEndpoint_INCLUDED
#define PizzaCollectionEndpoint_INCLUDED


#include "Pizzeria/Pizza.h"


namespace Pizzeria { 


//@ remote
//@ path="/pizzeria/pizzas"
class PizzaCollectionEndpoint
{
public:
	PizzaCollectionEndpoint(const std::vector<Pizza>& pizzas);
		/// Creates the PizzaCollectionEndpoint.

	std::vector<Pizza> get() const;
		/// Returns a vector containing the available pizzas.
		
private:
	std::vector<Pizza> _pizzas;
};


} // namespace Pizzeria


#endif // PizzaCollectionEndpoint_INCLUDED
