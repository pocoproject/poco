//
// PizzaPickupService.cpp
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
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
