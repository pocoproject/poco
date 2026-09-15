//
// JSObject.cpp
//
// Library: RemotingNG/REST
// Package: JSValue
// Module:  JSObject
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/JSObject.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


JSObject::JSObject()
{
}


JSObject::~JSObject()
{
}


JSValue::Type JSObject::type() const
{
	return JS_OBJECT;
}


std::size_t JSObject::size() const
{
	return _map.size();
}


const JSValue::Ptr& JSObject::operator [] (const std::string& name) const
{
	Map::const_iterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		throw Poco::NotFoundException(name);
}


JSValue::Ptr& JSObject::operator [] (const std::string& name)
{
	return _map[name];
}


bool JSObject::has(const std::string& name) const
{
	return _map.find(name) != _map.end();
}


} } } // namespace Poco::RemotingNG::REST
