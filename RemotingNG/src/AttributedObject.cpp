//
// AttributedObject.h
//
// Library: RemotingNG
// Package: ORB
// Module:  AttributedObject
//
// Definition of the AttributedObject class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/AttributedObject.h"
#include "Poco/Exception.h"


namespace Poco {
namespace RemotingNG {


AttributedObject::AttributedObject()
{
}


AttributedObject::AttributedObject(const AttributedObject& other):
	_attributes(other._attributes)
{
}


AttributedObject::AttributedObject(AttributedObject&& other) noexcept:
	_attributes(std::move(other._attributes))
{
}

	
AttributedObject& AttributedObject::operator = (const AttributedObject& other)
{
	if (&other != this)
	{
		_attributes = other._attributes;
	}
	return *this;
}


AttributedObject& AttributedObject::operator = (AttributedObject&& other) noexcept
{
	_attributes = std::move(other._attributes);
	return *this;
}


AttributedObject::~AttributedObject()
{
}

	
void AttributedObject::setAttribute(const std::string& name, const std::string& value)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	
	_attributes[name] = value;
}

	
const std::string& AttributedObject::getAttribute(const std::string& name) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	NameValueMap::const_iterator it = _attributes.find(name);
	if (it != _attributes.end())
		return it->second;
	else
		throw Poco::NotFoundException(name);
}

	
const std::string& AttributedObject::getAttribute(const std::string& name, const std::string& deflt) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	NameValueMap::const_iterator it = _attributes.find(name);
	if (it != _attributes.end())
		return it->second;
	else
		return deflt;
}


bool AttributedObject::hasAttribute(const std::string& name) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	NameValueMap::const_iterator it = _attributes.find(name);
	return it != _attributes.end();
}


void AttributedObject::removeAttribute(const std::string& name)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	_attributes.erase(name);
}


std::vector<std::string> AttributedObject::enumerateAttributes() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	std::vector<std::string> result;
	for (const auto& p: _attributes)
	{
		result.push_back(p.first);
	}
	
	return result;
}


void AttributedObject::clearAttributes()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	_attributes.clear();
}


} } // namespace Poco::RemotingNG
