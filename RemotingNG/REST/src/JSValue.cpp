//
// JSValue.cpp
//
// Library: RemotingNG/REST
// Package: JSValue
// Module:  JSValue
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/JSValue.h"
#include "Poco/Exception.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


std::size_t JSValue::size() const
{
	return 0;
}


const std::string& JSValue::stringValue() const
{
	throw Poco::NotImplementedException("Cannot create string representation of non-scalar JSValue");
}


const JSValue::Ptr& JSValue::operator [] (const std::string& name) const
{
	throw Poco::InvalidAccessException("Named value access is only supported for JSObject");
}


JSValue::Ptr& JSValue::operator [] (const std::string& name)
{
	throw Poco::InvalidAccessException("Named value access is only supported for JSObject");
}


const JSValue::Ptr& JSValue::operator [] (std::size_t index) const
{
	throw Poco::InvalidAccessException("Ordered value access is only supported for JSArray");
}


JSValue::Ptr& JSValue::operator [] (std::size_t index)
{
	throw Poco::InvalidAccessException("Ordered value access is only supported for JSArray");
}


bool JSValue::has(const std::string& name) const
{
	return false;
}


} } } // namespace Poco::RemotingNG::REST
