//
// IPizzaDeliveryService.h
//
// Package: Generated
// Module:  IPizzaDeliveryService
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef IPizzaDeliveryService_INCLUDED
#define IPizzaDeliveryService_INCLUDED


#include "Pizzeria/PizzaDeliveryService.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"


namespace Pizzeria {


class IPizzaDeliveryService: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IPizzaDeliveryService>;

	IPizzaDeliveryService();
		/// Creates a IPizzaDeliveryService.

	virtual ~IPizzaDeliveryService();
		/// Destroys the IPizzaDeliveryService.

	virtual const Pizzeria::Pizza& getAnyPizza() const = 0;

	virtual const Pizzeria::ExtTopping& getMostPopularTopping() const = 0;

	virtual const std::set<std::string>& getPizzaNames() const = 0;

	virtual const std::vector<Pizzeria::Pizza>& getPizzas() const = 0;
		/// Returns the different types of pizza which we offer.

	virtual const std::vector<Pizzeria::ExtTopping>& getToppings() const = 0;

	virtual Poco::Timespan getWaitTime() const = 0;
		/// Returns the approximate wait time.

	virtual Poco::DateTime order(const Pizzeria::Pizza& pizza, const Pizzeria::DeliveryAddress& deliverTo) = 0;
		/// Order a pizza, send it to the given deliveryAddress.
		/// Returns the expected time when the Pizza will arrive.

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace Pizzeria


#endif // IPizzaDeliveryService_INCLUDED

