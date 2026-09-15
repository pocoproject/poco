//
// PizzaDeliveryService.cpp
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Pizzeria/PizzaDeliveryService.h"
#include "Pizzeria/DeliveryAddress.h"
#include <iostream>


namespace Pizzeria {


PizzaDeliveryService::PizzaDeliveryService():
	_inventory(),
	_pizzaNames(),
	_toppings(),
	_waitTime(15*60, 0) // 15 minutes
{
	_toppings.push_back(ExtTopping("Salami", 80));
	_toppings.push_back(ExtTopping("Onion", 30));
	_toppings.push_back(ExtTopping("Ham", 80));
	_toppings.push_back(ExtTopping("Tabasco", 0));
	_toppings.push_back(ExtTopping("Cheese", 50));
	_toppings.push_back(ExtTopping("Egg", 40));
	_toppings.push_back(ExtTopping("Tuna", 70));
	_toppings.push_back(ExtTopping("Artichoke", 60));
	// lazy init
	_pizzaNames.insert("Salami");
	_pizzaNames.insert("Tuna");
	_pizzaNames.insert("Capricciosa");
	Pizza salami("Salami");
	salami.add(_toppings[0]);
	salami.add(_toppings[4]);
	salami.getDetails().calories += 50;
	salami.getDetails().percentFat += 8.2;
	salami.getDetails().carboHydrates += 3.3;
	salami.setBasePrice(600);
	Pizza tuna("Tuna");
	tuna.add(_toppings[6]);
	tuna.add(_toppings[4]);
	tuna.getDetails().calories += 40;
	tuna.getDetails().percentFat += 3.2;
	tuna.getDetails().carboHydrates += 3.3;
	tuna.setBasePrice(700);
	Pizza capricciosa("Capricciosa");
	capricciosa.add(_toppings[2]);
	capricciosa.add(_toppings[4]);
	capricciosa.add(_toppings[7]);
	capricciosa.getDetails().calories += 48;
	capricciosa.getDetails().percentFat += 7.8;
	capricciosa.getDetails().carboHydrates += 4.5;
	capricciosa.setBasePrice(550);
	_inventory.push_back(tuna);
	_inventory.push_back(salami);
	_inventory.push_back(capricciosa);
}


PizzaDeliveryService::~PizzaDeliveryService()
{
}


Poco::DateTime PizzaDeliveryService::order(const Pizza& pizza, const DeliveryAddress& deliverTo)
{
	std::cout << "Order for " << pizza.toString() << std::endl;
	std::cout << "Deliver to " << deliverTo.toString() << std::endl;

	Poco::DateTime now;
	now += getWaitTime();
	return now;
}


} // namespace Pizzeria
