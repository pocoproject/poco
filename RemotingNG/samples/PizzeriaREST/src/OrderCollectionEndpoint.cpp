//
// OrderCollectionEndpoint.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Pizzeria/OrderCollectionEndpoint.h"
#include "Pizzeria/OrderCollection.h"


namespace Pizzeria { 


OrderCollectionEndpoint::OrderCollectionEndpoint(OrderCollection& orderCollection):
	_orderCollection(orderCollection)
{
}


OrderCollectionEndpoint::~OrderCollectionEndpoint()
{
}


Order OrderCollectionEndpoint::post(const Order& order)
{
	return _orderCollection.add(order);
}


std::vector<Order> OrderCollectionEndpoint::get(int maxOrders) const
{
	return _orderCollection.list(maxOrders);
}


} // namespace Pizzeria
