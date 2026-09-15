//
// PizzaCollectionEndpointProxy.h
//
// Package: Generated
// Module:  PizzaCollectionEndpointProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef PizzaCollectionEndpointProxy_INCLUDED
#define PizzaCollectionEndpointProxy_INCLUDED


#include "Pizzeria/IPizzaCollectionEndpoint.h"
#include "Poco/RemotingNG/Proxy.h"


namespace Pizzeria {


class PizzaCollectionEndpointProxy: public Pizzeria::IPizzaCollectionEndpoint, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<PizzaCollectionEndpointProxy>;

	PizzaCollectionEndpointProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a PizzaCollectionEndpointProxy.

	virtual ~PizzaCollectionEndpointProxy();
		/// Destroys the PizzaCollectionEndpointProxy.

	std::vector < Pizzeria::Pizza > get() const;
		/// Returns a vector containing the available pizzas.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
	mutable std::vector < Pizzeria::Pizza > _getRet;
};


inline const Poco::RemotingNG::Identifiable::TypeId& PizzaCollectionEndpointProxy::remoting__typeId() const
{
	return IPizzaCollectionEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // PizzaCollectionEndpointProxy_INCLUDED

