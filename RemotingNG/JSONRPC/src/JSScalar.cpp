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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/JSONRPC/JSScalar.h"


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


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


} } } // namespace Poco::RemotingNG::JSONRPC
