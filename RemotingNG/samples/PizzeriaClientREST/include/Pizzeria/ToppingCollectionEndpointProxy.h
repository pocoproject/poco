//
// ToppingCollectionEndpointProxy.h
//
// Package: Generated
// Module:  ToppingCollectionEndpointProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef ToppingCollectionEndpointProxy_INCLUDED
#define ToppingCollectionEndpointProxy_INCLUDED


#include "Pizzeria/IToppingCollectionEndpoint.h"
#include "Poco/RemotingNG/Proxy.h"


namespace Pizzeria {


class ToppingCollectionEndpointProxy: public Pizzeria::IToppingCollectionEndpoint, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<ToppingCollectionEndpointProxy>;

	ToppingCollectionEndpointProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a ToppingCollectionEndpointProxy.

	virtual ~ToppingCollectionEndpointProxy();
		/// Destroys the ToppingCollectionEndpointProxy.

	std::vector < Pizzeria::ExtTopping > get() const;
		/// Returns a vector containing the available toppings.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
	mutable std::vector < Pizzeria::ExtTopping > _getRet;
};


inline const Poco::RemotingNG::Identifiable::TypeId& ToppingCollectionEndpointProxy::remoting__typeId() const
{
	return IToppingCollectionEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // ToppingCollectionEndpointProxy_INCLUDED

