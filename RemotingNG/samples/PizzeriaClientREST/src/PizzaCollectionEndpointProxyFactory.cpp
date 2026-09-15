//
// PizzaCollectionEndpointProxyFactory.cpp
//
// Package: Generated
// Module:  PizzaCollectionEndpointProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaCollectionEndpointProxyFactory.h"


namespace Pizzeria {


PizzaCollectionEndpointProxyFactory::PizzaCollectionEndpointProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


PizzaCollectionEndpointProxyFactory::~PizzaCollectionEndpointProxyFactory()
{
}


Poco::RemotingNG::Proxy* PizzaCollectionEndpointProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new Pizzeria::PizzaCollectionEndpointProxy(oid);
}


} // namespace Pizzeria

