//
// PizzaDeliveryServiceProxyFactory.h
//
// Package: Generated
// Module:  PizzaDeliveryServiceProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaDeliveryServiceProxyFactory_INCLUDED
#define PizzaDeliveryServiceProxyFactory_INCLUDED


#include "Pizzeria/PizzaDeliveryServiceProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


namespace Pizzeria {


class PizzaDeliveryServiceProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	PizzaDeliveryServiceProxyFactory();
		/// Creates a PizzaDeliveryServiceProxyFactory.

	~PizzaDeliveryServiceProxyFactory();
		/// Destroys the PizzaDeliveryServiceProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace Pizzeria


#endif // PizzaDeliveryServiceProxyFactory_INCLUDED

