//
// Order.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Order
//
// Definition of the Order class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Order_INCLUDED
#define XSDTypes_Order_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/CollectionContent.h"


namespace Poco::XSD::Types {


class XSDTypes_API Order: public CollectionContent
	/// Abstract super class for collections of OrderContent elements.
{
public:
	using Ptr = AutoPtr<Order>;

	Order(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the Order.

	~Order() override;
		/// Destroys the Order.

	virtual void add(OrderContent::Ptr pChild) = 0;
		/// Adds a child to the order.
};


} // namespace Poco::XSD::Types


#endif // XSDTypes_Order_INCLUDED
