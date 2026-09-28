//
// Any.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Any
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Any.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/AnyIterator.h"
#include "Poco/XSD/Types/OrderIterator.h"


namespace Poco::XSD::Types {


Any::Any(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc, const std::string& ns, ProcessStyle style):
	OrderContent(id, minOcc, maxOcc),
	_nameSpace(ns),
	_style(style)
{
}


Any::~Any() = default;


void Any::fixup()
{
}


void Any::accept(Visitor& v) const
{
	v.visit(*this);
}


OrderIterator Any::iterator() const
{
	return OrderIterator(new AnyIterator(*const_cast<Any*>(this)));
}


} // namespace Poco::XSD::Types
