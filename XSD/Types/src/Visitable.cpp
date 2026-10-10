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
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Visitable.h"


namespace Poco::XSD::Types {


Visitable::Visitable() = default;


Visitable::Visitable(const std::string& id):
	_id(id)
{
}


Visitable::~Visitable() = default;


} // namespace Poco::XSD::Types
