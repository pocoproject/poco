//
// Binding.cpp
//
// Library: XSD/Types
// Package: WSDL
// Module:  Binding
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Binding.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco::XSD::Types {


Binding::Binding() = default;


Binding::Binding(const std::string& name):
	_name(name)
{
}


Binding::~Binding() = default;


void Binding::accept(Visitor& v) const
{
	v.visit(*this);
}


} // namespace Poco::XSD::Types
