//
// SimpleType.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  SimpleType
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco::XSD::Types {


SimpleType::SimpleType() = default;


SimpleType::SimpleType(const std::string& id, const std::string& name, bool finalRestriction, bool finalList, bool finalUnion):
	Type(id, name),
	_finalRestriction(finalRestriction),
	_finalList(finalList),
	_finalUnion(finalUnion)
{
}


SimpleType::~SimpleType() = default;


void SimpleType::fixup()
{
	if (_pContent)
	{
		_pContent->fixup();
	}
	else
	{
		// a builtin simpletype has no parents and thus no content
	}
}


const std::vector<const Type*>& SimpleType::parents() const
{
	if (_pContent)
		return _pContent->types();

	return NOPARENTS;
}


void SimpleType::accept(Visitor& v) const
{
	v.visit(*this);
}


void SimpleType::createIterator(std::vector<OrderIterator>& seq) const
{
	// a simple type has no children
}


} // namespace Poco::XSD::Types
