//
// OrderEndpoint.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Pizzeria/OrderEndpoint.h"
#include "Pizzeria/OrderCollection.h"


namespace Pizzeria { 


OrderEndpoint::OrderEndpoint(OrderCollection& orderCollection):
	_orderCollection(orderCollection)
{
}


OrderEndpoint::~OrderEndpoint()
{
}


Order OrderEndpoint::get(int orderNumber) const
{
	return _orderCollection.get(orderNumber);
}

	
Order OrderEndpoint::patch(int orderNumber, const Order& order)
{
	return _orderCollection.update(orderNumber, order);
}


void OrderEndpoint::delete_(int orderNumber)
{
	_orderCollection.remove(orderNumber);
}


} // namespace Pizzeria
