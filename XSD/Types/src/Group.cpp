//
// Group.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Group
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/Group.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


Group::Group(const std::string& id, const std::string& name, Poco::UInt32 minOcc, Poco::UInt32 maxOcc):
	AbstractGroup(id, minOcc, maxOcc),
	_name(name)
{
}


Group::~Group()
{
}


void Group::fixup()
{
	if (_pChild)
	{
		_pChild->fixup();
	}
}


void Group::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
