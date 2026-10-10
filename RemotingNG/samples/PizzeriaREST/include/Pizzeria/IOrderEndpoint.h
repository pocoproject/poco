//
// IOrderEndpoint.h
//
// Package: Generated
// Module:  IOrderEndpoint
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef IOrderEndpoint_INCLUDED
#define IOrderEndpoint_INCLUDED


#include "Pizzeria/OrderEndpoint.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"


namespace Pizzeria {


class IOrderEndpoint: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IOrderEndpoint>;

	IOrderEndpoint();
		/// Creates a IOrderEndpoint.

	virtual ~IOrderEndpoint();
		/// Destroys the IOrderEndpoint.

	virtual void delete_(int orderNumber) = 0;
		/// Deletes the Order with the given ID.

	virtual Pizzeria::Order get(int orderNumber) const = 0;
		/// Returns the Order with the given orderId.

	virtual Pizzeria::Order patch(int orderNumber, const Pizzeria::Order& order) = 0;
		/// Changes the given order.

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace Pizzeria


#endif // IOrderEndpoint_INCLUDED

