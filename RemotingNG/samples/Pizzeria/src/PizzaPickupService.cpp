//
// PizzaPickupService.cpp
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Pizzeria/PizzaPickupService.h"
#include "Pizzeria/DeliveryAddress.h"
#include <iostream>


namespace Pizzeria {


PizzaPickupService::PizzaPickupService():PizzaDeliveryService()
{
}


PizzaPickupService::~PizzaPickupService()
{
}


Poco::DateTime PizzaPickupService::orderForSelfPickup(const Pizza& pizza, const std::string& phone)
{
	std::cout << "Order for " << pizza.toString() << std::endl;
	std::cout << "SELF Pickup, phone number: " << phone << std::endl;

	Poco::DateTime now;
	now += getWaitTime();
	return now;
}


} // namespace Pizzeria
