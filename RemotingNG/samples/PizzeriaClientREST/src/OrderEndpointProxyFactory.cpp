//
// OrderEndpointProxyFactory.cpp
//
// Package: Generated
// Module:  OrderEndpointProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderEndpointProxyFactory.h"


namespace Pizzeria {


OrderEndpointProxyFactory::OrderEndpointProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


OrderEndpointProxyFactory::~OrderEndpointProxyFactory()
{
}


Poco::RemotingNG::Proxy* OrderEndpointProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new Pizzeria::OrderEndpointProxy(oid);
}


} // namespace Pizzeria

