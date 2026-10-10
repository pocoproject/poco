//
// OrderEndpoint.h
//
// Definition of the OrderEndpoint class.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef OrderEndpoint_INCLUDED
#define OrderEndpoint_INCLUDED


#include "Pizzeria/Order.h"


namespace Pizzeria {


class OrderCollection;


//@ remote
//@ path="/pizzeria/orders/{orderNumber}"
class OrderEndpoint
{
public:
	OrderEndpoint(OrderCollection& orderCollection);
		/// Creates the OrderEndpoint.

	~OrderEndpoint();
		/// Destroys the OrderEndpoint.

	//@ $orderNumber={in=path, min=1}
	Order get(int orderNumber) const;
		/// Returns the Order with the given orderId.

	//@ $orderNumber={in=path, min=1}
	Order patch(int orderNumber, const Order& order);
		/// Changes the given order.

	//@ $orderNumber={in=path, min=1}
	void delete_(int orderNumber);
		/// Deletes the Order with the given ID.

private:
	OrderCollection& _orderCollection;
};


} // namespace Pizzeria


#endif // OrderEndpoint_INCLUDED
