//
// AbstractAttribute.cpp
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AbstractAttribute
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AbstractAttribute.h"


namespace Poco::XSD::Types {


AbstractAttribute::AbstractAttribute(const std::string& id, const std::string& name):
	AttributeContent(id, name)
{
}


AbstractAttribute::~AbstractAttribute() = default;


} // namespace Poco::XSD::Types
