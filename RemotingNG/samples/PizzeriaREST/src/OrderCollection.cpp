//
// OrderCollection.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Pizzeria/OrderCollection.h"
#include "Poco/Exception.h"


namespace Pizzeria { 


OrderCollection::OrderCollection():
	_nextOrderNumber(1)
{
}

	
OrderCollection::~OrderCollection()
{
}

	
Order OrderCollection::add(const Order& order)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	Poco::DateTime deliveryTime;
	deliveryTime += Poco::Timespan(20*60, 0);
	_orders.push_back(order);
	_orders.back().deliveryTime = deliveryTime;
	_orders.back().orderNumber = _nextOrderNumber++;
	return _orders.back();
}

	
Order OrderCollection::get(int orderNumber) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	
	for (std::vector<Order>::const_iterator it = _orders.begin(); it != _orders.end(); ++it)
	{
		if (it->orderNumber.value() == orderNumber)
			return *it;
	}
	throw Poco::NotFoundException();
}


Order OrderCollection::update(int orderNumber, const Order& newOrder)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	
	for (std::vector<Order>::iterator it = _orders.begin(); it != _orders.end(); ++it)
	{
		if (it->orderNumber.value() == orderNumber)
		{
			it->pizza = newOrder.pizza;
			it->deliveryAddress = newOrder.deliveryAddress;
			return *it;
		}
	}
	throw Poco::NotFoundException();
}

	
void OrderCollection::remove(int orderNumber)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	for (std::vector<Order>::iterator it = _orders.begin(); it != _orders.end(); ++it)
	{
		if (it->orderNumber.value() == orderNumber)
		{
			_orders.erase(it);
			return;
		}
	}
	throw Poco::NotFoundException();
}

	
std::vector<Order> OrderCollection::list(int maxOrders) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	
	std::vector<Order> result;
	for (std::vector<Order>::const_reverse_iterator it = _orders.rbegin(); maxOrders-- > 0 && it != _orders.rend(); ++it)
	{
		result.push_back(*it);
	}
	
	return result;
}


} // namespace Pizzeria
