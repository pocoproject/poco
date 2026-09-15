//
// Context.cpp
//
// Library: RemotingNG
// Package: Transport
// Module:  Context
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/Context.h"
#include "Poco/Exception.h"


namespace Poco {
namespace RemotingNG {


Poco::ThreadLocal<Context::Ptr> Context::_pContextHolder;


Context::Context()
{
}


Context::~Context()
{
}


void Context::set(Context::Ptr pContext)
{
	*_pContextHolder = pContext;
}

	
Context::Ptr Context::get()
{
	return *_pContextHolder;
}

	
bool Context::has(const std::string& key) const
{
	return _attrs.find(key) != _attrs.end();
}


const Poco::Any& Context::get(const std::string& key) const
{
	Attributes::const_iterator it = _attrs.find(key);
	if (it != _attrs.end())
		return it->second;
	else
		throw Poco::NotFoundException(key);
}


void Context::set(const std::string& key, const Poco::Any& value)
{
	_attrs[key] = value;
}


Context::Attributes::const_iterator Context::find(const std::string& key) const
{
	return _attrs.find(key);
}


Context::Attributes::const_iterator Context::begin() const
{
	return _attrs.begin();
}


Context::Attributes::const_iterator Context::end() const
{
	return _attrs.end();
}


void Context::setCredentials(const Credentials& creds)
{
	_creds = creds;
}


void Context::clearCredentials()
{
	_creds.clearAttributes();
}


ScopedContext::ScopedContext():
	_pContext(new Context)
{
	Context::set(_pContext);
}


ScopedContext::~ScopedContext()
{
	_pContext = 0;
	try
	{
		Context::set(_pContext);
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} } // namespace Poco::RemotingNG
