//
// Order.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Order
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Order.h"


namespace Poco {
namespace XSD {
namespace Types {


Order::Order(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	CollectionContent(id, minOcc, maxOcc)
{
}


Order::~Order()
{
}


} } } // namespace Poco::XSD::Types
