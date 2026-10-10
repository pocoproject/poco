//
// PizzaPickupServiceProxyFactory.h
//
// Package: Generated
// Module:  PizzaPickupServiceProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaPickupServiceProxyFactory_INCLUDED
#define PizzaPickupServiceProxyFactory_INCLUDED


#include "Pizzeria/PizzaPickupServiceProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


namespace Pizzeria {


class PizzaPickupServiceProxyFactory: public Poco::RemotingNG::ProxyFactory
	/// Pizzeria that allows self pickup.
{
public:
	PizzaPickupServiceProxyFactory();
		/// Creates a PizzaPickupServiceProxyFactory.

	~PizzaPickupServiceProxyFactory();
		/// Destroys the PizzaPickupServiceProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace Pizzeria


#endif // PizzaPickupServiceProxyFactory_INCLUDED

