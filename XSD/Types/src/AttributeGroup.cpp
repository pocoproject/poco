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


namespace Poco {
namespace XSD {
namespace Types {


AttributeGroup::AttributeGroup(const std::string& id, const std::string& name):
	AbstractAttributeGroup(id, name)
{
}


AttributeGroup::~AttributeGroup()
{
}


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
	std::vector<AttributeContent::Ptr>::iterator it = _tmp.begin();
	for (; it != _tmp.end(); ++it)
	{
		(*it)->fixup();
		std::pair<AbstractAttributeGroup::Attributes::iterator, bool> res = _children.insert(std::make_pair((*it)->name(), *it));
		if (!res.second)
			throw SchemaException("Duplicate attribute " + (*it)->name());
	}
	_tmp.clear();
}


} } } // namespace Poco::XSD::Types
