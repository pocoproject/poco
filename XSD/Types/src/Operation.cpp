//
// Operation.cpp
//
// Library: XSD/Types
// Package: WSDL
// Module:  Operation
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Operation.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/StringTokenizer.h"


namespace Poco::XSD::Types {


Operation::Operation() = default;


Operation::Operation(const std::string& name):
	_name(name)
{
}


Operation::~Operation() = default;

	
void Operation::accept(Visitor& v) const
{
	v.visit(*this);
}


void Operation::setParameterOrder(const std::string& parameterOrder)
{
	Poco::StringTokenizer tok(parameterOrder, " \t\r\n", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
	_parameterOrder.assign(tok.begin(), tok.end());
}


} // namespace Poco::XSD::Types
