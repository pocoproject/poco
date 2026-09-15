//
// OrderEndpointProxy.h
//
// Package: Generated
// Module:  OrderEndpointProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderEndpointProxy_INCLUDED
#define OrderEndpointProxy_INCLUDED


#include "Pizzeria/IOrderEndpoint.h"
#include "Poco/RemotingNG/Proxy.h"


namespace Pizzeria {


class OrderEndpointProxy: public Pizzeria::IOrderEndpoint, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<OrderEndpointProxy>;

	OrderEndpointProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a OrderEndpointProxy.

	virtual ~OrderEndpointProxy();
		/// Destroys the OrderEndpointProxy.

	void delete_(int orderNumber);
		/// Deletes the Order with the given ID.

	Pizzeria::Order get(int orderNumber) const;
		/// Returns the Order with the given orderId.

	Pizzeria::Order patch(int orderNumber, const Pizzeria::Order& order);
		/// Changes the given order.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
	mutable Pizzeria::Order _getRet;
	mutable Pizzeria::Order _patchRet;
};


inline const Poco::RemotingNG::Identifiable::TypeId& OrderEndpointProxy::remoting__typeId() const
{
	return IOrderEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // OrderEndpointProxy_INCLUDED

