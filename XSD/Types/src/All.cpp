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


namespace Poco {
namespace XSD {
namespace Types {


All::All(const std::string& id, Poco::UInt32 minOcc):
	Order(id, minOcc, 1)
{
}


All::~All()
{
}


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

	_content.insert(std::make_pair(pChild->name(), pChild));
}


void All::fixup()
{
	All::Content::iterator it = _content.begin();
	All::Content::iterator itEnd = _content.end();
	for (; it != itEnd; ++it)
		it->second->fixup();
}


void All::accept(Visitor& v) const
{
	v.visit(*this);
}


OrderIterator All::iterator() const
{
	return OrderIterator(new AllIterator(*this));
}


} } } // namespace Poco::XSD::Types
