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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
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
