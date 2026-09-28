//
// AttributeRef.cpp
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeRef
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AttributeRef.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco {
namespace XSD {
namespace Types {


AttributeRef::AttributeRef(const std::string& id, const QName& ref):
	AbstractAttribute(id, ""),
	_ref(ref),
	_pAttr(nullptr)
{
}


AttributeRef::AttributeRef(const std::string& id, const QName& ref, const AbstractAttribute* pAttr):
	AbstractAttribute(id, ""),
	_ref(ref),
	_pAttr(pAttr)
{
	poco_check_ptr (pAttr);
	if (dynamic_cast<const AttributeRef*>(_pAttr) != nullptr)
			throw XSDException("AttributeRef can't reference another AttributeRef: possible infinite recursion");
}


AttributeRef::~AttributeRef()
{
}


const std::string& AttributeRef::nameSpace() const
{
	return _ref.getNamespace();
}


const std::string& AttributeRef::defaultValue() const
{
	poco_assert_dbg (_pAttr);
	return _pAttr->defaultValue();
}


const std::string& AttributeRef::fixedValue() const
{
	poco_assert_dbg (_pAttr);
	return _pAttr->fixedValue();
}


bool AttributeRef::qualifiedForm() const
{
	poco_assert_dbg (_pAttr);
	return _pAttr->qualifiedForm();
}


const SimpleType* AttributeRef::type() const
{
	poco_assert_dbg (_pAttr);
	return _pAttr->type();
}


const std::string& AttributeRef::name() const
{
	poco_assert_dbg (_pAttr);
	return _pAttr->name();
}


void AttributeRef::fixup()
{
	if (!_pAttr)
	{
		_pAttr = TypesManager::instance().getAttribute(_ref);
		if (!_pAttr)
			throw InvalidTypeException("AttributeRef references invalid attribute:" + _ref.name());
		// check for possible infinite ref loop
		if (dynamic_cast<const AttributeRef*>(_pAttr) != nullptr)
			throw XSDException("AttributeRef can't reference another AttributeRef: possible infinite recursion");
	}
}


AbstractAttribute::Usage AttributeRef::usage() const
{
	poco_assert_dbg (_pAttr);
	return _pAttr->usage();
}


void AttributeRef::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
