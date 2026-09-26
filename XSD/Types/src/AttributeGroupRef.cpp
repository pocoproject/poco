//
// AttributeGroupRef.cpp
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeGroupRef
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AttributeGroupRef.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco {
namespace XSD {
namespace Types {


AttributeGroupRef::AttributeGroupRef(const std::string& id, const QName& ref):
	AbstractAttributeGroup(id, ref.name()),
	_ref(ref),
	_pGroup(0)
{
}


AttributeGroupRef::~AttributeGroupRef()
{
}


const AbstractAttributeGroup::Attributes& AttributeGroupRef::getAttributes() const
{
	poco_assert_dbg (_pGroup);
	return _pGroup->getAttributes();
}


bool AttributeGroupRef::hasAnyAttribute() const
{
	poco_assert_dbg (_pGroup);
	return _pGroup->hasAnyAttribute();
}


AnyAttribute::Ptr AttributeGroupRef::getAny() const
{
	poco_assert_dbg (_pGroup);
	return _pGroup->getAny();
}


void AttributeGroupRef::fixup()
{
	if (!_pGroup)
	{
		_pGroup = TypesManager::instance().getAttributeGroup(_ref);
		if (!_pGroup)
			throw InvalidTypeException("AttributeGroupRef references invalid attribute group:" + _ref.name());
		// check for possible infinite ref loop
		if (dynamic_cast<const AttributeGroupRef*>(_pGroup) != 0)
			throw XSDException("AttributeGroupRef can't reference another AttributeGroupRef: possible infinite recursion");
	}
}


void AttributeGroupRef::accept(Visitor& v) const
{
	poco_assert_dbg (_pGroup);
}


} } } // namespace Poco::XSD::Types
