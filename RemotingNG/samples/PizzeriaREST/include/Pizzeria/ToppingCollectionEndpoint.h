//
// ToppingCollectionEndpoint.h
//
// Definition of the ToppingCollectionEndpoint class.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef ToppingCollectionEndpoint_INCLUDED
#define ToppingCollectionEndpoint_INCLUDED


#include "Pizzeria/Topping.h"


namespace Pizzeria { 


//@ remote
//@ path="/pizzeria/toppings"
class ToppingCollectionEndpoint
{
public:
	ToppingCollectionEndpoint(const std::vector<ExtTopping>& toppings);
		/// Creates the ToppingCollectionEndpoint.

	std::vector<ExtTopping> get() const;
		/// Returns a vector containing the available toppings.
		
private:
	std::vector<ExtTopping> _toppings;
};


} // namespace Pizzeria


#endif // ToppingCollectionEndpoint_INCLUDED
