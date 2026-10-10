//
// OrderCollectionEndpointProxy.h
//
// Package: Generated
// Module:  OrderCollectionEndpointProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderCollectionEndpointProxy_INCLUDED
#define OrderCollectionEndpointProxy_INCLUDED


#include "Pizzeria/IOrderCollectionEndpoint.h"
#include "Poco/RemotingNG/Proxy.h"


namespace Pizzeria {


class OrderCollectionEndpointProxy: public Pizzeria::IOrderCollectionEndpoint, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<OrderCollectionEndpointProxy>;

	OrderCollectionEndpointProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a OrderCollectionEndpointProxy.

	virtual ~OrderCollectionEndpointProxy();
		/// Destroys the OrderCollectionEndpointProxy.

	std::vector < Pizzeria::Order > get(int maxOrders = int(10)) const;
		/// Returns a vector containing the most recent orders.
		/// A maximum number of orders to be returned can be specified.

	Pizzeria::Order post(const Pizzeria::Order& order);
		/// Creates a new Order.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
	mutable std::vector < Pizzeria::Order > _getRet;
	mutable Pizzeria::Order _postRet;
};


inline const Poco::RemotingNG::Identifiable::TypeId& OrderCollectionEndpointProxy::remoting__typeId() const
{
	return IOrderCollectionEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // OrderCollectionEndpointProxy_INCLUDED

