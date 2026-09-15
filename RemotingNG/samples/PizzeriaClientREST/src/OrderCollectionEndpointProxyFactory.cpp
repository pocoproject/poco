//
// OrderCollectionEndpointProxyFactory.cpp
//
// Package: Generated
// Module:  OrderCollectionEndpointProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderCollectionEndpointProxyFactory.h"


namespace Pizzeria {


OrderCollectionEndpointProxyFactory::OrderCollectionEndpointProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


OrderCollectionEndpointProxyFactory::~OrderCollectionEndpointProxyFactory()
{
}


Poco::RemotingNG::Proxy* OrderCollectionEndpointProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new Pizzeria::OrderCollectionEndpointProxy(oid);
}


} // namespace Pizzeria

