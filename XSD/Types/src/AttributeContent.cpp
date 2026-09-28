//
// AttributeContent.cpp
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeContent
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AttributeContent.h"


namespace Poco::XSD::Types {


AttributeContent::AttributeContent(const std::string& id, const std::string& name):
	AnnotatedObject(id),
	_name(name)
{
}


AttributeContent::~AttributeContent() = default;


} // namespace Poco::XSD::Types
