//
// JSArray.cpp
//
// Library: RemotingNG/REST
// Package: JSValue
// Module:  JSArray
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/JSArray.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


JSArray::JSArray()
{
}


JSArray::~JSArray()
{
}


JSValue::Type JSArray::type() const
{
	return JS_ARRAY;
}


std::size_t JSArray::size() const
{
	return _vector.size();
}


const JSValue::Ptr& JSArray::operator [] (std::size_t index) const
{
	if (index < _vector.size())
		return _vector[index];
	else
		throw Poco::InvalidAccessException("index out of range");
}


JSValue::Ptr& JSArray::operator [] (std::size_t index)
{
	if (index < _vector.size())
	{
		return _vector[index];
	}
	else if (index == _vector.size())
	{
		_vector.push_back(JSValue::Ptr());
		return _vector.back();
	}
	else throw Poco::InvalidAccessException("index out of range");
}


} } } // namespace Poco::RemotingNG::REST
