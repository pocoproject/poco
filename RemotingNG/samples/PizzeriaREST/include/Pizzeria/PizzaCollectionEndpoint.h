//
// PizzaCollectionEndpoint.h
//
// Definition of the PizzaCollectionEndpoint class.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
