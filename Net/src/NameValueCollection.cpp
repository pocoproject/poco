//
// NameValueCollection.cpp
//
// Library: Net
// Package: Messages
// Module:  NameValueCollection
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/NameValueCollection.h"
#include "Poco/Exception.h"
#include <algorithm>


using Poco::NotFoundException;


namespace Poco {
namespace Net {


NameValueCollection::NameValueCollection()
{
}


NameValueCollection::NameValueCollection(const NameValueCollection& nvc):
	_map(nvc._map)
{
}


NameValueCollection::NameValueCollection(NameValueCollection&& nvc) noexcept:
	_map(std::move(nvc._map))
{
}


NameValueCollection::~NameValueCollection()
{
}


NameValueCollection& NameValueCollection::operator = (const NameValueCollection& nvc)
{
	NameValueCollection tmp(nvc);
	swap(tmp);

	return *this;
}


NameValueCollection& NameValueCollection::operator = (NameValueCollection&& nvc) noexcept
{
	_map = std::move(nvc._map);

	return *this;
}


void NameValueCollection::swap(NameValueCollection& nvc) noexcept
{
	std::swap(_map, nvc._map);
}


const std::string& NameValueCollection::operator [] (const std::string& name) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		throw NotFoundException(name);
}


void NameValueCollection::set(const std::string& name, const std::string& value)
{
	Iterator it = _map.find(name);
	if (it != _map.end())
		it->second = value;
	else
		_map.insert(HeaderMap::ValueType(name, value));
}


void NameValueCollection::add(const std::string& name, const std::string& value)
{
	_map.insert(HeaderMap::ValueType(name, value));
}


const std::string& NameValueCollection::get(const std::string& name) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		throw NotFoundException(name);
}


const std::string& NameValueCollection::get(const std::string& name, const std::string& defaultValue) const
{
	ConstIterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		return defaultValue;
}


bool NameValueCollection::has(const std::string& name) const
{
	return _map.find(name) != _map.end();
}


NameValueCollection::ConstIterator NameValueCollection::find(const std::string& name) const
{
	return _map.find(name);
}


NameValueCollection::ConstIterator NameValueCollection::begin() const
{
	return _map.begin();
}


NameValueCollection::ConstIterator NameValueCollection::end() const
{
	return _map.end();
}


bool NameValueCollection::empty() const
{
	return _map.empty();
}


std::size_t NameValueCollection::size() const
{
	return _map.size();
}


void NameValueCollection::erase(const std::string& name)
{
	_map.erase(name);
}


void NameValueCollection::secureErase(const std::string& name)
{
	Iterator it = _map.find(name);
	while (it != _map.end())
	{
		Poco::secureClear(it->second);
		_map.erase(it);
		it = _map.find(name);
	}
}


void NameValueCollection::clear()
{
	_map.clear();
}


void NameValueCollection::secureClear()
{
	Iterator it = _map.begin();
	while (it != _map.end())
	{
		Poco::secureClear(it->second);
		++it;
	}
	_map.clear();
}


} } // namespace Poco::Net
