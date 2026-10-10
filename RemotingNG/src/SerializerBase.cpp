//
// SerializerBase.cpp
//
// Library: RemotingNG
// Package: Serialization
// Module:  SerializerBase
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/SerializerBase.h"
#include "Poco/Exception.h"


namespace Poco {
namespace RemotingNG {


const std::string SerializerBase::PROP_NAMESPACE("namespace");
const std::string SerializerBase::PROP_HEADER("header");
const std::string SerializerBase::PROP_ACTION("action");
const std::string SerializerBase::PROP_REQUEST("request");
const std::string SerializerBase::PROP_REPLY("reply");
const std::string SerializerBase::PROP_FAULT("fault");
const std::string SerializerBase::PROP_LOCATION("location");
const std::string SerializerBase::PROP_FORMAT("format");
const std::string SerializerBase::PROP_PATH("path");
const std::string SerializerBase::PROP_PRODUCES("produces");
const std::string SerializerBase::PROP_CONSUMES("consumes");
const std::string SerializerBase::PROP_CONTENT_TYPE("contentType");
const std::string SerializerBase::PROP_LENGTH("length");
const std::string SerializerBase::PROP_XSDTYPE("xsdType");
const std::string SerializerBase::RETURN_PARAM("return");


SerializerBase::SerializerBase()
{
}


SerializerBase::~SerializerBase()
{
}


void SerializerBase::pushProperty(const std::string& name, const std::string& value)
{
	Properties::iterator it = _props.find(name);
	if (it == _props.end())
	{
		it = _props.insert(std::make_pair(name, ValueStack())).first;
	}
	it->second.push(value);
}


void SerializerBase::popProperty(const std::string& name)
{
	Properties::iterator it = _props.find(name);
	if (it != _props.end() && !it->second.empty())
		it->second.pop();
	else
		throw Poco::IllegalStateException("No property value to pop", name);
}


const std::string& SerializerBase::getProperty(const std::string& name) const
{
	Properties::const_iterator it = _props.find(name);
	if (it != _props.end() && !it->second.empty())
		return it->second.top();
	else
		throw Poco::NotFoundException("Serializer property", name);
}


const std::string& SerializerBase::getProperty(const std::string& name, const std::string& deflt) const
{
	Properties::const_iterator it = _props.find(name);
	if (it != _props.end() && !it->second.empty())
		return it->second.top();
	else
		return deflt;
}


bool SerializerBase::hasProperty(const std::string& name) const
{
	Properties::const_iterator it = _props.find(name);
	return (it != _props.end() && !it->second.empty());
}


void SerializerBase::clearProperties()
{
	_props.clear();
}


} } // namespace Poco::RemotingNG
