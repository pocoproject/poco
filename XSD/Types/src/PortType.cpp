//
// PortType.cpp
//
// Library: XSD/Types
// Package: WSDL
// Module:  PortType
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/PortType.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco::XSD::Types {


PortType::PortType() = default;


PortType::PortType(const std::string& name):
	_name(name)
{
}


PortType::~PortType() = default;

	
void PortType::addOperation(const Operation::Ptr pOperation)
{
	_operations[pOperation->name()] = pOperation;
}


void PortType::accept(Visitor& v) const
{
	v.visit(*this);
}


} // namespace Poco::XSD::Types
