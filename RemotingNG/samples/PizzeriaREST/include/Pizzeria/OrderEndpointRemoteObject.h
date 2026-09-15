//
// OrderEndpointRemoteObject.h
//
// Package: Generated
// Module:  OrderEndpointRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef OrderEndpointRemoteObject_INCLUDED
#define OrderEndpointRemoteObject_INCLUDED


#include "Pizzeria/IOrderEndpoint.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


class OrderEndpointRemoteObject: public Pizzeria::IOrderEndpoint, public Poco::RemotingNG::RemoteObject
{
public:
	using Ptr = Poco::AutoPtr<OrderEndpointRemoteObject>;

	OrderEndpointRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Pizzeria::OrderEndpoint> pServiceObject);
		/// Creates a OrderEndpointRemoteObject.

	virtual ~OrderEndpointRemoteObject();
		/// Destroys the OrderEndpointRemoteObject.

	void delete_(int orderNumber);
		/// Deletes the Order with the given ID.

	Pizzeria::Order get(int orderNumber) const;
		/// Returns the Order with the given orderId.

	Pizzeria::Order patch(int orderNumber, const Pizzeria::Order& order);
		/// Changes the given order.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	Poco::SharedPtr<Pizzeria::OrderEndpoint> _pServiceObject;
};


inline void OrderEndpointRemoteObject::delete_(int orderNumber)
{
	_pServiceObject->delete_(orderNumber);
}


inline Pizzeria::Order OrderEndpointRemoteObject::get(int orderNumber) const
{
	return _pServiceObject->get(orderNumber);
}


inline Pizzeria::Order OrderEndpointRemoteObject::patch(int orderNumber, const Pizzeria::Order& order)
{
	return _pServiceObject->patch(orderNumber, order);
}


inline const Poco::RemotingNG::Identifiable::TypeId& OrderEndpointRemoteObject::remoting__typeId() const
{
	return IOrderEndpoint::remoting__typeId();
}


} // namespace Pizzeria


#endif // OrderEndpointRemoteObject_INCLUDED

