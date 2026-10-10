//
// Order.h
//
// Definition of the Order struct.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Order_INCLUDED
#define Order_INCLUDED


#include "Pizzeria/Pizza.h"
#include "Pizzeria/DeliveryAddress.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/DateTime.h"


namespace Pizzeria {


//@ serialize
struct Order
{
	//@ min=1, max=100
	Poco::Optional<int> orderNumber;
	Pizza pizza;
	DeliveryAddress deliveryAddress;
	Poco::Nullable<Poco::DateTime> deliveryTime;
};


} // namespace Pizzeria


#endif // Order_INCLUDED
