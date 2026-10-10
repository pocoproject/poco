//
// Sequence.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Sequence
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Sequence.h"
#include "Poco/XSD/Types/SequenceIterator.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco::XSD::Types {


Sequence::Sequence(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	Order(id, minOcc, maxOcc)
{
}


Sequence::~Sequence() = default;


void Sequence::accept(Visitor& v) const
{
	v.visit(*this);
}


void Sequence::add(OrderContent::Ptr pChild)
{
	if (!pChild)
		throw NullTypeException("Cannot add null child to Sequence");
	_content.push_back(pChild);
}


void Sequence::fixup()
{
	for (auto& pChild: _content)
		pChild->fixup();
}


OrderIterator Sequence::iterator() const
{
	return OrderIterator(new SequenceIterator(*this));
}


} // namespace Poco::XSD::Types
