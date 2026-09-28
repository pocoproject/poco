//
// All.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  All
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/All.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/OrderIterator.h"
#include "Poco/XSD/Types/AllIterator.h"


namespace Poco::XSD::Types {


All::All(const std::string& id, Poco::UInt32 minOcc):
	Order(id, minOcc, 1)
{
}


All::~All() = default;


void All::add(OrderContent::Ptr pChild)
{
	Element::Ptr  ptr = pChild.cast<Element>();
	add(ptr);
}


void All::add(Element::Ptr pChild)
{
	if (!pChild)
		throw NullElementException("Cannot add null element to All");

	poco_assert(!pChild->name().empty());

	_content.try_emplace(pChild->name(), pChild);
}


void All::fixup()
{
	for (auto& [name, pElem]: _content)
		pElem->fixup();
}


void All::accept(Visitor& v) const
{
	v.visit(*this);
}


OrderIterator All::iterator() const
{
	return OrderIterator(new AllIterator(*this));
}


} // namespace Poco::XSD::Types
