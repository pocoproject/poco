//
// Visitable.cpp
//
// Library: XSD/Types
// Package: Visitor
// Module:  Visitable
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/Visitable.h"


namespace Poco {
namespace XSD {
namespace Types {


Visitable::Visitable()
{
}


Visitable::Visitable(const std::string& id):
	_id(id)
{
}


Visitable::~Visitable()
{
}


} } } // namespace Poco::XSD::Types
