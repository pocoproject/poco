//
// ToppingCollectionEndpointProxyFactory.h
//
// Package: Generated
// Module:  ToppingCollectionEndpointProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef ToppingCollectionEndpointProxyFactory_INCLUDED
#define ToppingCollectionEndpointProxyFactory_INCLUDED


#include "Pizzeria/ToppingCollectionEndpointProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


namespace Pizzeria {


class ToppingCollectionEndpointProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	ToppingCollectionEndpointProxyFactory();
		/// Creates a ToppingCollectionEndpointProxyFactory.

	~ToppingCollectionEndpointProxyFactory();
		/// Destroys the ToppingCollectionEndpointProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace Pizzeria


#endif // ToppingCollectionEndpointProxyFactory_INCLUDED

