//
// PizzaDeliveryServiceProxy.h
//
// Package: Generated
// Module:  PizzaDeliveryServiceProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaDeliveryServiceProxy_INCLUDED
#define PizzaDeliveryServiceProxy_INCLUDED


#include "Pizzeria/IPizzaDeliveryService.h"
#include "Poco/RemotingNG/Proxy.h"


namespace Pizzeria {


class PizzaDeliveryServiceProxy: public Pizzeria::IPizzaDeliveryService, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<PizzaDeliveryServiceProxy>;

	PizzaDeliveryServiceProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a PizzaDeliveryServiceProxy.

	virtual ~PizzaDeliveryServiceProxy();
		/// Destroys the PizzaDeliveryServiceProxy.

	virtual void getAnyPizza(Pizzeria::Pizza& return_);

	virtual void getMostPopularTopping(Pizzeria::ExtTopping& return_);

	virtual void getPizzaNames(std::vector<std::string>& return_);

	virtual void getPizzas(std::vector<Pizzeria::Pizza>& return_);

	virtual void getToppings(std::vector<Pizzeria::ExtTopping>& return_);

	virtual void getWaitTime(Poco::Int64& return_);

	virtual void order(const Pizzeria::DeliveryAddress& deliverTo, const Pizzeria::Pizza& pizza, Poco::DateTime& return_);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& PizzaDeliveryServiceProxy::remoting__typeId() const
{
	return IPizzaDeliveryService::remoting__typeId();
}


} // namespace Pizzeria


#endif // PizzaDeliveryServiceProxy_INCLUDED

