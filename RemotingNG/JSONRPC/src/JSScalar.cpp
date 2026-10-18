//
// JSScalar.cpp
//
// Library: RemotingNG/JSONRPC
// Package: JSValue
// Module:  JSScalar
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/JSONRPC/JSScalar.h"


namespace Poco::RemotingNG::JSONRPC {


JSScalar::JSScalar(Type type):
	_type(type)
{
}


JSScalar::JSScalar(const std::string& value, Type type):
	_value(value),
	_type(type)
{
}


JSScalar::~JSScalar() = default;


JSValue::Type JSScalar::type() const
{
	return _type;
}


const std::string& JSScalar::stringValue() const
{
	return _value;
}


} // namespace Poco::RemotingNG::JSONRPC
