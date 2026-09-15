//
// OrderCollectionEndpointRemoteObject.h
//
// Package: Generated
// Module:  OrderCollectionEndpointRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderCollectionEndpointRemoteObject_INCLUDED
#define OrderCollectionEndpointRemoteObject_INCLUDED


#include "Pizzeria/IOrderCollectionEndpoint.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


class OrderCollectionEndpointRemoteObject: public Pizzeria::IOrderCollectionEndpoint, public Poco::RemotingNG::RemoteObject
{
public:
	using Ptr = Poco::AutoPtr<OrderCollectionEndpointRemoteObject>;

	OrderCollectionEndpointRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::OrderCollectionEndpoint> pServiceObject);
		/// Creates a OrderCollectionEndpointRemoteObject.

	virtual ~OrderCollectionEndpointRemoteObject();
		/// Destroys the OrderCollectionEndpointRemoteObject.

	std::vector<Pizzeria::Order> get(int maxOrders = int(10)) const;
		/// Returns a vector containing the most recent orders.
		/// A maximum number of orders to be returned can be specified.

	Pizzeria::Order post(const Pizzeria::Order& order);
		/// Creates a new Order.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	Poco::SharedPtr<Pizzeria::OrderCollectionEndpoint> _pServiceObject;
};


inline std::vector<Pizzeria::Order> OrderCollectionEndpointRemoteObject::get(int maxOrders) const
{
	return _pServiceObject->get(maxOrders);
}


inline Pizzeria::Order OrderCollectionEndpointRemoteObject::post(const Pizzeria::Order& order)
{
	return _pServiceObject->post(order);
}


inline const Poco::RemotingNG::Identifiable::TypeId& OrderCollectionEndpointRemoteObject::remoting__typeId() const
{
	return IOrderCollectionEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // OrderCollectionEndpointRemoteObject_INCLUDED

