//
// PizzaDeliveryService.h
//
// Definition of the PizzaDeliveryService class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef PizzaDeliveryService_INCLUDED
#define PizzaDeliveryService_INCLUDED


#include "Pizzeria/Pizza.h"
#include "Poco/Random.h"
#include "Poco/DateTime.h"
#include "Poco/Timespan.h"
#include <set>


namespace Pizzeria { 


class DeliveryAddress;


//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/", cacheExpire="5min"
class PizzaDeliveryService
{
public:
	PizzaDeliveryService();
		/// Creates the PizzaDeliveryService.

	virtual ~PizzaDeliveryService();
		/// Destroys the PizzaDeliveryService.
	
	//@ remote, cacheResult=true
	Poco::Timespan getWaitTime() const;
		/// Returns the approximate wait time.

	void setWaitTime(const Poco::Timespan& span);
		/// Sets the wait time. This method should not be accessible remote.

	//@ remote, cacheResult, cacheExpire = "8h"
	const std::vector<Pizza>& getPizzas() const;
		/// Returns the different types of pizza which we offer.

	//@ remote
	const Pizza& getAnyPizza() const;

	//@ remote
	const ExtTopping& getMostPopularTopping() const;

	//@ remote, cacheResult, cacheExpire="infinite"
	const ExtToppings& getToppings() const;

	//@ remote
	//@ synchronized = "true"
	//@ $pizza = {order = 2}
	//@ $deliverTo = {order = 1}
	virtual Poco::DateTime order(const Pizza& pizza, const DeliveryAddress& deliverTo);
		/// Order a pizza, send it to the given deliveryAddress.
		/// Returns the expected time when the Pizza will arrive.

	//@ remote, cacheResult, cacheExpire = "8h"
	const std::set<std::string>& getPizzaNames() const;

private:
	std::vector<Pizza> _inventory; /// all standard pizzas
	std::set<std::string> _pizzaNames;
	ExtToppings _toppings; /// all supported toppings
	Poco::Timespan _waitTime;

};


inline Poco::Timespan PizzaDeliveryService::getWaitTime() const
{
	return _waitTime;
}


inline void PizzaDeliveryService::setWaitTime(const Poco::Timespan& waitTime)
{
	_waitTime = waitTime;
}


inline const std::vector<Pizza>& PizzaDeliveryService::getPizzas() const
{
	return _inventory;
}


inline const Pizza& PizzaDeliveryService::getAnyPizza() const
{
	return _inventory[0];
}


inline const ExtTopping& PizzaDeliveryService::getMostPopularTopping() const
{
	return _toppings[0];
}


inline const ExtToppings& PizzaDeliveryService::getToppings() const
{
	return _toppings;
}


inline const std::set<std::string>& PizzaDeliveryService::getPizzaNames() const
{
	return _pizzaNames;
}


} // namespace Pizzeria


#endif // PizzaDeliveryService_INCLUDED
