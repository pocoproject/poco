//
// SimpleRestriction.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  SimpleRestriction
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/SimpleRestriction.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


SimpleRestriction::SimpleRestriction(const std::string& id, const QName& baseClass):
	SimpleTypeInheritance(id),
	_baseClass(baseClass),
	_baseType()
{
}


SimpleRestriction::~SimpleRestriction()
{
}


void SimpleRestriction::fixup()
{
	if (_baseType.empty())
	{
		// find the element, throw exception if not found
		const Type* pBaseType = TypesManager::instance().getType(_baseClass);
		if (!pBaseType)
			throw NullTypeException("Failed to resolve type reference " + _baseClass.name());
		_baseType.push_back(pBaseType);
	}
}


void SimpleRestriction::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
