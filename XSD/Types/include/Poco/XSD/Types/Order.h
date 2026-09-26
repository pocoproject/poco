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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_Order_INCLUDED
#define XSDTypes_Order_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/CollectionContent.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Order: public CollectionContent
	/// Abstract super class for collections of OrderContent elements.
{
public:
	using Ptr = AutoPtr<Order>;

	Order(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the Order.

	virtual ~Order();
		/// Destroys the Order.

	virtual void add(OrderContent::Ptr pChild) = 0;
		/// Adds a child to the order.
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Order_INCLUDED
