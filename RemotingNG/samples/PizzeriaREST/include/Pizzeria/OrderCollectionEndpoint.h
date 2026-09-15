//
// OrderCollectionEndpoint.h
//
// Definition of the OrderCollectionEndpoint class.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef OrderCollectionEndpoint_INCLUDED
#define OrderCollectionEndpoint_INCLUDED


#include "Pizzeria/Order.h"


namespace Pizzeria {


class OrderCollection;


//@ remote
//@ path="/pizzeria/orders"
class OrderCollectionEndpoint
{
public:
	OrderCollectionEndpoint(OrderCollection& orderCollection);
		/// Creates the OrderCollectionEndpoint.

	~OrderCollectionEndpoint();
		/// Destroys the OrderCollectionEndpoint.

	Order post(const Order& order);
		/// Creates a new Order.

	//@ $maxOrders={in=query, optional}
	std::vector<Order> get(int maxOrders = 10) const;
		/// Returns a vector containing the most recent orders.
		/// A maximum number of orders to be returned can be specified.

private:
	OrderCollection& _orderCollection;
};


} // namespace Pizzeria


#endif // OrderCollectionEndpoint_INCLUDED
