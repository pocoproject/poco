//
// OrderCollectionEndpointProxyFactory.h
//
// Package: Generated
// Module:  OrderCollectionEndpointProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderCollectionEndpointProxyFactory_INCLUDED
#define OrderCollectionEndpointProxyFactory_INCLUDED


#include "Pizzeria/OrderCollectionEndpointProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


namespace Pizzeria {


class OrderCollectionEndpointProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	OrderCollectionEndpointProxyFactory();
		/// Creates a OrderCollectionEndpointProxyFactory.

	~OrderCollectionEndpointProxyFactory();
		/// Destroys the OrderCollectionEndpointProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace Pizzeria


#endif // OrderCollectionEndpointProxyFactory_INCLUDED

