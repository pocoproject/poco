//
// IOrderCollectionEndpoint.h
//
// Package: Generated
// Module:  IOrderCollectionEndpoint
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef IOrderCollectionEndpoint_INCLUDED
#define IOrderCollectionEndpoint_INCLUDED


#include "Pizzeria/OrderCollectionEndpoint.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"


namespace Pizzeria {


class IOrderCollectionEndpoint: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IOrderCollectionEndpoint>;

	IOrderCollectionEndpoint();
		/// Creates a IOrderCollectionEndpoint.

	virtual ~IOrderCollectionEndpoint();
		/// Destroys the IOrderCollectionEndpoint.

	virtual std::vector<Pizzeria::Order> get(int maxOrders = int(10)) const = 0;
		/// Returns a vector containing the most recent orders.
		/// A maximum number of orders to be returned can be specified.

	virtual Pizzeria::Order post(const Pizzeria::Order& order) = 0;
		/// Creates a new Order.

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace Pizzeria


#endif // IOrderCollectionEndpoint_INCLUDED

