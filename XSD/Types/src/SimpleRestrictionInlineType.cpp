//
// SimpleRestrictionInlineType.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  SimpleRestrictionInlineType
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/SimpleRestrictionInlineType.h"
#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco::XSD::Types {


SimpleRestrictionInlineType::SimpleRestrictionInlineType(const std::string& id):
	SimpleTypeInheritance(id),
	_baseType()
{
}


SimpleRestrictionInlineType::~SimpleRestrictionInlineType()
{
	if (_pInlineType)
		_pInlineType->release();
}


void SimpleRestrictionInlineType::setType(SimpleType::Ptr ptr)
{
	poco_check_ptr (ptr);
	if (_pInlineType != nullptr)
		throw SchemaException("A restriction must not declare more than one inline simpleType");
	_pInlineType = ptr;
	_pInlineType->duplicate();
	_baseType.clear();
	_baseType.push_back(_pInlineType);
}


void SimpleRestrictionInlineType::fixup()
{
	if (_pInlineType == nullptr)
		throw SchemaException("A restriction must have a base attribute or an inline simpleType");
	_pInlineType->fixup();
}


void SimpleRestrictionInlineType::accept(Visitor& v) const
{
	v.visit(*this);
}


} // namespace Poco::XSD::Types
