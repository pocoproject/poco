//
// MediaType.cpp
//
// Library: Net
// Package: Messages
// Module:  MediaType
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/MediaType.h"
#include "Poco/Ascii.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/String.h"
#include <algorithm>
#include <utility>


using Poco::icompare;


namespace Poco {
namespace Net {


MediaType::MediaType(const std::string& mediaType)
{
	parse(mediaType);
}


MediaType::MediaType(std::string  type, std::string  subType):
	_type(std::move(type)),
	_subType(std::move(subType))
{
}


MediaType::MediaType(const MediaType& mediaType)
	
= default;


MediaType::MediaType(MediaType&& mediaType) noexcept:
	_type(std::move(mediaType._type)),
	_subType(std::move(mediaType._subType)),
	_parameters(std::move(mediaType._parameters))
{
}


MediaType::~MediaType()
= default;


MediaType& MediaType::operator = (const MediaType& mediaType)
{
	if (&mediaType != this)
	{
		_type       = mediaType._type;
		_subType    = mediaType._subType;
		_parameters = mediaType._parameters;
	}
	return *this;
}


MediaType& MediaType::operator = (MediaType&& mediaType) noexcept
{
	_type       = std::move(mediaType._type);
	_subType    = std::move(mediaType._subType);
	_parameters = std::move(mediaType._parameters);

	return *this;
}


MediaType& MediaType::operator = (const std::string& mediaType)
{
	parse(mediaType);
	return *this;
}


void MediaType::swap(MediaType& mediaType) noexcept
{
	std::swap(_type, mediaType._type);
	std::swap(_subType, mediaType._subType);
	_parameters.swap(mediaType._parameters);
}


void MediaType::setType(const std::string& type)
{
	_type = type;
}


void MediaType::setSubType(const std::string& subType)
{
	_subType = subType;
}


void MediaType::setParameter(const std::string& name, const std::string& value)
{
	_parameters.set(name, value);
}


const std::string& MediaType::getParameter(const std::string& name) const
{
	return _parameters.get(name);
}


bool MediaType::hasParameter(const std::string& name) const
{
	return _parameters.has(name);
}


void MediaType::removeParameter(const std::string& name)
{
	_parameters.erase(name);
}


std::string MediaType::toString() const
{
	std::string result;
	result.append(_type);
	result.append("/");
	result.append(_subType);
	for (const auto & _parameter : _parameters)
	{
		result.append("; ");
		result.append(_parameter.first);
		result.append("=");
		MessageHeader::quote(_parameter.second, result);
	}
	return result;
}


bool MediaType::matches(const MediaType& mediaType) const
{
	return matches(mediaType._type, mediaType._subType);
}


bool MediaType::matches(const std::string& type, const std::string& subType) const
{
	return icompare(_type, type) == 0 && icompare(_subType, subType) == 0;
}


bool MediaType::matches(const std::string& type) const
{
	return icompare(_type, type) == 0;
}


bool MediaType::matchesRange(const MediaType& mediaType) const
{
	return matchesRange(mediaType._type, mediaType._subType);
}


bool MediaType::matchesRange(const std::string& type, const std::string& subType) const
{
	if (_type == "*" || type == "*" || icompare(_type, type) == 0)
	{
		return _subType == "*" || subType == "*" || icompare(_subType, subType) == 0;
	}
	else return false;
}


bool MediaType::matchesRange(const std::string& type) const
{
	return _type == "*" || type == "*" || matches(type);
}


void MediaType::parse(const std::string& mediaType)
{
	_type.clear();
	_subType.clear();
	_parameters.clear();
	std::string::const_iterator it  = mediaType.begin();
	std::string::const_iterator end = mediaType.end();
	while (it != end && Poco::Ascii::isSpace(*it)) ++it;
	while (it != end && *it != '/') _type += *it++;
	if (it != end) ++it;
	while (it != end && *it != ';' && !Poco::Ascii::isSpace(*it)) _subType += *it++;
	while (it != end && *it != ';') ++it;
	if (it != end) ++it; // skip semicolon
	MessageHeader::splitParameters(it, end, _parameters);
}


} } // namespace Poco::Net
