//
// AnyAttribute.cpp
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AnyAttribute
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AnyAttribute.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


AnyAttribute::AnyAttribute(const std::string& id, const std::string& nameSpace, ProcessStyle style):
	AttributeContent(id, "*"),
	_nameSpace(nameSpace),
	_style(style)
{
}


AnyAttribute::~AnyAttribute()
{
}


void AnyAttribute::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
