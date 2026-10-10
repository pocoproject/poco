//
// List.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  List
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/List.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Type.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco::XSD::Types {


List::List(const std::string& id):
	AbstractList(id),
	_pSimple(),
	_types()
{
}


List::~List() = default;


void List::fixup()
{
	poco_check_ptr (_pSimple);
	_pSimple->fixup();
}


void List::setType(SimpleType::Ptr pSimple)
{
	poco_check_ptr (pSimple);
	_pSimple = pSimple;
	const Type* pType = _pSimple.get();
	_types.clear();
	_types.push_back(pType);
}


void List::accept(Visitor& v) const
{
	v.visit(*this);
}


} // namespace Poco::XSD::Types
