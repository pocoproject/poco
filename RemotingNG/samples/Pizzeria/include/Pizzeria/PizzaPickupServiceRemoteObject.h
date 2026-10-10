//
// PizzaPickupServiceRemoteObject.h
//
// Package: Generated
// Module:  PizzaPickupServiceRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaPickupServiceRemoteObject_INCLUDED
#define PizzaPickupServiceRemoteObject_INCLUDED


#include "Pizzeria/IPizzaPickupService.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


class PizzaPickupServiceRemoteObject: public Pizzeria::IPizzaPickupService, public Poco::RemotingNG::RemoteObject
	/// Pizzeria that allows self pickup.
{
public:
	using Ptr = Poco::AutoPtr<PizzaPickupServiceRemoteObject>;

	PizzaPickupServiceRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::PizzaPickupService> pServiceObject);
		/// Creates a PizzaPickupServiceRemoteObject.

	virtual ~PizzaPickupServiceRemoteObject();
		/// Destroys the PizzaPickupServiceRemoteObject.

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

	Poco::DateTime orderForSelfPickup(const Pizzeria::Pizza& pizza, const std::string& phoneNumber);
		/// Order a pizza, send it to the given deliveryAddress.
		/// Returns the expected time when the Pizza will arrive.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	Poco::SharedPtr<Pizzeria::PizzaPickupService> _pServiceObject;
};


inline const Pizzeria::Pizza& PizzaPickupServiceRemoteObject::getAnyPizza() const
{
	return _pServiceObject->getAnyPizza();
}


inline const Pizzeria::ExtTopping& PizzaPickupServiceRemoteObject::getMostPopularTopping() const
{
	return _pServiceObject->getMostPopularTopping();
}


inline const std::set<std::string>& PizzaPickupServiceRemoteObject::getPizzaNames() const
{
	return _pServiceObject->getPizzaNames();
}


inline const std::vector<Pizzeria::Pizza>& PizzaPickupServiceRemoteObject::getPizzas() const
{
	return _pServiceObject->getPizzas();
}


inline const std::vector<Pizzeria::ExtTopping>& PizzaPickupServiceRemoteObject::getToppings() const
{
	return _pServiceObject->getToppings();
}


inline Poco::Timespan PizzaPickupServiceRemoteObject::getWaitTime() const
{
	return _pServiceObject->getWaitTime();
}


inline Poco::DateTime PizzaPickupServiceRemoteObject::order(const Pizzeria::Pizza& pizza, const Pizzeria::DeliveryAddress& deliverTo)
{
	Poco::FastMutex::ScopedLock lock(mutex());
	return _pServiceObject->order(pizza, deliverTo);
}


inline Poco::DateTime PizzaPickupServiceRemoteObject::orderForSelfPickup(const Pizzeria::Pizza& pizza, const std::string& phoneNumber)
{
	Poco::FastMutex::ScopedLock lock(mutex());
	return _pServiceObject->orderForSelfPickup(pizza, phoneNumber);
}


inline const Poco::RemotingNG::Identifiable::TypeId& PizzaPickupServiceRemoteObject::remoting__typeId() const
{
	return IPizzaPickupService::remoting__typeId();
}


} // namespace Pizzeria


#endif // PizzaPickupServiceRemoteObject_INCLUDED

