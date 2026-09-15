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

	virtual void getAnyPizza(Pizzeria::Pizza& return_) = 0;

	virtual void getMostPopularTopping(Pizzeria::ExtTopping& return_) = 0;

	virtual void getPizzaNames(std::vector<std::string>& return_) = 0;

	virtual void getPizzas(std::vector<Pizzeria::Pizza>& return_) = 0;

	virtual void getToppings(std::vector<Pizzeria::ExtTopping>& return_) = 0;

	virtual void getWaitTime(Poco::Int64& return_) = 0;

	virtual void order(const Pizzeria::DeliveryAddress& deliverTo, const Pizzeria::Pizza& pizza, Poco::DateTime& return_) = 0;

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace Pizzeria


#endif // IPizzaDeliveryService_INCLUDED

