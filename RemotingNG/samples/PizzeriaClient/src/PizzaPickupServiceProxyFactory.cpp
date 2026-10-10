//
// PizzaPickupServiceProxyFactory.cpp
//
// Package: Generated
// Module:  PizzaPickupServiceProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaPickupServiceProxyFactory.h"


namespace Pizzeria {


PizzaPickupServiceProxyFactory::PizzaPickupServiceProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


PizzaPickupServiceProxyFactory::~PizzaPickupServiceProxyFactory()
{
}


Poco::RemotingNG::Proxy* PizzaPickupServiceProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new Pizzeria::PizzaPickupServiceProxy(oid);
}


} // namespace Pizzeria

