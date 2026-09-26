//
// AbstractList.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  AbstractList
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/AbstractList.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco {
namespace XSD {
namespace Types {


AbstractList::AbstractList(const std::string& id):
	SimpleTypeInheritance(id)
{
}


AbstractList::~AbstractList()
{
}


} } } // namespace Poco::XSD::Types
