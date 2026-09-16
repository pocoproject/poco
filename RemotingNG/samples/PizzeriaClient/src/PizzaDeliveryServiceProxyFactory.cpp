//
// PizzaDeliveryServiceProxyFactory.cpp
//
// Package: Generated
// Module:  PizzaDeliveryServiceProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaDeliveryServiceProxyFactory.h"


namespace Pizzeria {


PizzaDeliveryServiceProxyFactory::PizzaDeliveryServiceProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


PizzaDeliveryServiceProxyFactory::~PizzaDeliveryServiceProxyFactory()
{
}


Poco::RemotingNG::Proxy* PizzaDeliveryServiceProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new Pizzeria::PizzaDeliveryServiceProxy(oid);
}


} // namespace Pizzeria

