//
// AbstractAttributeGroup.cpp
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AbstractAttributeGroup
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AbstractAttributeGroup.h"


namespace Poco::XSD::Types {


AbstractAttributeGroup::AbstractAttributeGroup(const std::string& id, const std::string& name):
	AttributeContent(id, name)
{
}


AbstractAttributeGroup::~AbstractAttributeGroup() = default;


} // namespace Poco::XSD::Types
