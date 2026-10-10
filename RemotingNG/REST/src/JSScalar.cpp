//
// JSScalar.cpp
//
// Library: RemotingNG/REST
// Package: JSValue
// Module:  JSScalar
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/JSScalar.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


JSScalar::JSScalar(Type type):
	_type(type)
{
}


JSScalar::JSScalar(const std::string& value, Type type):
	_value(value),
	_type(type)
{
}


JSScalar::~JSScalar()
{
}


JSValue::Type JSScalar::type() const
{
	return _type;
}


const std::string& JSScalar::stringValue() const
{
	return _value;
}


} } } // namespace Poco::RemotingNG::REST
