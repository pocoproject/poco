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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/Sequence.h"
#include "Poco/XSD/Types/SequenceIterator.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco {
namespace XSD {
namespace Types {


Sequence::Sequence(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	Order(id, minOcc, maxOcc)
{
}


Sequence::~Sequence()
{
}


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
	std::vector<OrderContent::Ptr>::iterator it = _content.begin();
	std::vector<OrderContent::Ptr>::iterator itEnd = _content.end();
	for (; it != itEnd; ++it)
		(*it)->fixup();
}


OrderIterator Sequence::iterator() const
{
	return OrderIterator(new SequenceIterator(*this));
}


} } } // namespace Poco::XSD::Types
