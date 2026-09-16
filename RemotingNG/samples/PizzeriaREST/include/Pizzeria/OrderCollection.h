//
// OrderCollection.h
//
// Definition of the OrderCollection class.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef OrderCollection_INCLUDED
#define OrderCollection_INCLUDED


#include "Pizzeria/Order.h"
#include "Poco/Mutex.h"
#include <vector>


namespace Pizzeria { 


class OrderCollection
{
public:
	OrderCollection();
		/// Creates the OrderCollection.
		
	~OrderCollection();
		/// Destroys the OrderCollection.
		
	Order add(const Order& order);
		/// Adds an Order.
		
	Order get(int orderNumber) const;
		/// Returns the Order with the given orderNumber.
		
	Order update(int orderNumber, const Order& newOrder);
		/// Updates an Order.
		
	void remove(int orderNumber);
		/// Deletes the Order with the given orderNumber.
		
	std::vector<Order> list(int maxOrders) const;	
		/// Returns a vector containing the most recent order, at most maxOrders.
		
private:
	std::vector<Order> _orders;
	int _nextOrderNumber;
	mutable Poco::FastMutex _mutex;
};


} // namespace Pizzeria


#endif // OrderCollection_INCLUDED
