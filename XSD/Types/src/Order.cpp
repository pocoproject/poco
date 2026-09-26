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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
