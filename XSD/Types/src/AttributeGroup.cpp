//
// AttributeGroup.cpp
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeGroup
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AttributeGroup.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco::XSD::Types {


AttributeGroup::AttributeGroup(const std::string& id, const std::string& name):
	AbstractAttributeGroup(id, name)
{
}


AttributeGroup::~AttributeGroup() = default;


void AttributeGroup::add(AttributeContent::Ptr ptr)
{
	if (!ptr)
		throw NullPointerException("AttributeGroup: adding null content");

	if (ptr->isAny())
	{
		_pAny = ptr.cast<AnyAttribute>();
		poco_assert_dbg (_pAny);
		return;
	}

	_tmp.push_back(ptr);
}


void AttributeGroup::accept(Visitor& v) const
{
	v.visit(*this);
}


void AttributeGroup::fixup()
{
	for (auto& pAttr: _tmp)
	{
		pAttr->fixup();
		if (!_children.try_emplace(pAttr->name(), pAttr).second)
			throw SchemaException("Duplicate attribute " + pAttr->name());
	}
	_tmp.clear();
}


} // namespace Poco::XSD::Types
