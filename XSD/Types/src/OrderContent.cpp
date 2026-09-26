//
// OrderContent.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  OrderContent
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/OrderContent.h"
#include "Poco/XSD/Types/Type.h"


namespace Poco {
namespace XSD {
namespace Types {


OrderContent::OrderContent():
	_id(),
	_minOccurs(1),
	_maxOccurs(1)
{
}


OrderContent::OrderContent(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	_id(id),
	_minOccurs(minOcc),
	_maxOccurs(maxOcc)
{
	poco_assert (_minOccurs <= _maxOccurs);
}


OrderContent::~OrderContent()
{
}


const std::vector<const Type*>& OrderContent::types() const
{
	return Type::NOPARENTS;
}


} } } // namespace Poco::XSD::Types
