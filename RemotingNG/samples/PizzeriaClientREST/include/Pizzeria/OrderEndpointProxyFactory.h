//
// OrderEndpointProxyFactory.h
//
// Package: Generated
// Module:  OrderEndpointProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderEndpointProxyFactory_INCLUDED
#define OrderEndpointProxyFactory_INCLUDED


#include "Pizzeria/OrderEndpointProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


namespace Pizzeria {


class OrderEndpointProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	OrderEndpointProxyFactory();
		/// Creates a OrderEndpointProxyFactory.

	~OrderEndpointProxyFactory();
		/// Destroys the OrderEndpointProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace Pizzeria


#endif // OrderEndpointProxyFactory_INCLUDED

