//
// PizzaCollectionEndpointProxyFactory.h
//
// Package: Generated
// Module:  PizzaCollectionEndpointProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaCollectionEndpointProxyFactory_INCLUDED
#define PizzaCollectionEndpointProxyFactory_INCLUDED


#include "Pizzeria/PizzaCollectionEndpointProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


namespace Pizzeria {


class PizzaCollectionEndpointProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	PizzaCollectionEndpointProxyFactory();
		/// Creates a PizzaCollectionEndpointProxyFactory.

	~PizzaCollectionEndpointProxyFactory();
		/// Destroys the PizzaCollectionEndpointProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace Pizzeria


#endif // PizzaCollectionEndpointProxyFactory_INCLUDED

