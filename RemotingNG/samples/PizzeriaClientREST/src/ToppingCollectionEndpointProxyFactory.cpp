//
// ToppingCollectionEndpointProxyFactory.cpp
//
// Package: Generated
// Module:  ToppingCollectionEndpointProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/ToppingCollectionEndpointProxyFactory.h"


namespace Pizzeria {


ToppingCollectionEndpointProxyFactory::ToppingCollectionEndpointProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


ToppingCollectionEndpointProxyFactory::~ToppingCollectionEndpointProxyFactory()
{
}


Poco::RemotingNG::Proxy* ToppingCollectionEndpointProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new Pizzeria::ToppingCollectionEndpointProxy(oid);
}


} // namespace Pizzeria

