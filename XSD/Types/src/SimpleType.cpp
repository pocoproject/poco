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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


SimpleType::SimpleType()
{
}


SimpleType::SimpleType(const std::string& id, const std::string& name, bool finalRestriction, bool finalList, bool finalUnion):
	Type(id, name),
	_finalRestriction(finalRestriction),
	_finalList(finalList),
	_finalUnion(finalUnion)
{
}


SimpleType::~SimpleType()
{
}


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


} } } // namespace Poco::XSD::Types
