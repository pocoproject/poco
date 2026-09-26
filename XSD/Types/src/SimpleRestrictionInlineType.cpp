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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/SimpleRestrictionInlineType.h"
#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


SimpleRestrictionInlineType::SimpleRestrictionInlineType(const std::string& id):
	SimpleTypeInheritance(id),
	_baseType(),
	_pInlineType(0)
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
	poco_assert (!_pInlineType);
	_pInlineType = ptr;
	_pInlineType->duplicate();
	_baseType.clear();
	_baseType.push_back(_pInlineType);
}


void SimpleRestrictionInlineType::fixup()
{
	_pInlineType->fixup();
}


void SimpleRestrictionInlineType::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
