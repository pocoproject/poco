//
// AbstractGroup.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  AbstractGroup
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AbstractGroup.h"
#include "Poco/XSD/Types/OrderIterator.h"
#include "Poco/XSD/Types/Sequence.h"


namespace Poco {
namespace XSD {
namespace Types {


AbstractGroup::AbstractGroup(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	CollectionContent(id, minOcc, maxOcc)
{
}


AbstractGroup::~AbstractGroup()
{
}


OrderIterator AbstractGroup::iterator() const
{
	Order::Ptr ptr = getChild();
	if (ptr)
		return ptr->iterator();

	//dummy sequence
	Sequence::Ptr pSeq = new Sequence("", 0, 1);
	return pSeq->iterator();
}


} } } // namespace Poco::XSD::Types
