//
// SimpleTypeInheritance.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  SimpleTypeInheritance
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/SimpleTypeInheritance.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco::XSD::Types {


SimpleTypeInheritance::SimpleTypeInheritance() = default;


SimpleTypeInheritance::SimpleTypeInheritance(const std::string& id):
	AnnotatedObject(id)
{
}


SimpleTypeInheritance::~SimpleTypeInheritance() = default;


} // namespace Poco::XSD::Types
