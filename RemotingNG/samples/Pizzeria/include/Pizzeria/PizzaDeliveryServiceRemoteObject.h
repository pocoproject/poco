//
// PizzaDeliveryServiceRemoteObject.h
//
// Package: Generated
// Module:  PizzaDeliveryServiceRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaDeliveryServiceRemoteObject_INCLUDED
#define PizzaDeliveryServiceRemoteObject_INCLUDED


#include "Pizzeria/IPizzaDeliveryService.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


class PizzaDeliveryServiceRemoteObject: public Pizzeria::IPizzaDeliveryService, public Poco::RemotingNG::RemoteObject
{
public:
	using Ptr = Poco::AutoPtr<PizzaDeliveryServiceRemoteObject>;

	PizzaDeliveryServiceRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::PizzaDeliveryService> pServiceObject);
		/// Creates a PizzaDeliveryServiceRemoteObject.

	virtual ~PizzaDeliveryServiceRemoteObject();
		/// Destroys the PizzaDeliveryServiceRemoteObject.

	const Pizzeria::Pizza& getAnyPizza() const;

	const Pizzeria::ExtTopping& getMostPopularTopping() const;

	const std::set<std::string>& getPizzaNames() const;

	const std::vector<Pizzeria::Pizza>& getPizzas() const;
		/// Returns the different types of pizza which we offer.

	const std::vector<Pizzeria::ExtTopping>& getToppings() const;

	Poco::Timespan getWaitTime() const;
		/// Returns the approximate wait time.

	virtual Poco::DateTime order(const Pizzeria::Pizza& pizza, const Pizzeria::DeliveryAddress& deliverTo);
		/// Order a pizza, send it to the given deliveryAddress.
		/// Returns the expected time when the Pizza will arrive.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	Poco::SharedPtr<Pizzeria::PizzaDeliveryService> _pServiceObject;
};


inline const Pizzeria::Pizza& PizzaDeliveryServiceRemoteObject::getAnyPizza() const
{
	return _pServiceObject->getAnyPizza();
}


inline const Pizzeria::ExtTopping& PizzaDeliveryServiceRemoteObject::getMostPopularTopping() const
{
	return _pServiceObject->getMostPopularTopping();
}


inline const std::set<std::string>& PizzaDeliveryServiceRemoteObject::getPizzaNames() const
{
	return _pServiceObject->getPizzaNames();
}


inline const std::vector<Pizzeria::Pizza>& PizzaDeliveryServiceRemoteObject::getPizzas() const
{
	return _pServiceObject->getPizzas();
}


inline const std::vector<Pizzeria::ExtTopping>& PizzaDeliveryServiceRemoteObject::getToppings() const
{
	return _pServiceObject->getToppings();
}


inline Poco::Timespan PizzaDeliveryServiceRemoteObject::getWaitTime() const
{
	return _pServiceObject->getWaitTime();
}


inline Poco::DateTime PizzaDeliveryServiceRemoteObject::order(const Pizzeria::Pizza& pizza, const Pizzeria::DeliveryAddress& deliverTo)
{
	Poco::FastMutex::ScopedLock lock(mutex());
	return _pServiceObject->order(pizza, deliverTo);
}


inline const Poco::RemotingNG::Identifiable::TypeId& PizzaDeliveryServiceRemoteObject::remoting__typeId() const
{
	return IPizzaDeliveryService::remoting__typeId();
}


} // namespace Pizzeria


#endif // PizzaDeliveryServiceRemoteObject_INCLUDED

