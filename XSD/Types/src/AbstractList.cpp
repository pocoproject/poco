//
// AbstractList.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  AbstractList
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AbstractList.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco::XSD::Types {


AbstractList::AbstractList(const std::string& id):
	SimpleTypeInheritance(id)
{
}


AbstractList::~AbstractList() = default;


} // namespace Poco::XSD::Types
