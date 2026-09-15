//
// RESTServerException.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  RESTServerException
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/RESTServerException.h"
#include "Poco/RemotingNG/REST/JSONSerializer.h"


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace REST {


RESTServerException::RESTServerException(const std::string& reason, int status, JSValue::Ptr pContent):
	Poco::RemotingNG::RemoteException(reason, status),
	_reason(reason),
	_pContent(pContent)
{
}


RESTServerException::RESTServerException(const std::string& reason, const std::string& message, int status, JSValue::Ptr pContent):
	Poco::RemotingNG::RemoteException(reason, message, status),
	_reason(reason),
	_pContent(pContent)
{
}


RESTServerException::RESTServerException(const RESTServerException& exc):
	Poco::RemotingNG::RemoteException(exc),
	_pContent(exc._pContent)
{
}


RESTServerException::~RESTServerException() noexcept
{
}


RESTServerException& RESTServerException::operator = (const RESTServerException& exc)
{
	if (&exc != this)
	{
		RemoteException::operator = (exc);
		_pContent = exc._pContent;
	}
	return *this;
}


const char* RESTServerException::name() const noexcept
{
	return "REST Server Error";
}


const char* RESTServerException::className() const noexcept
{
	return typeid(*this).name();
}


Poco::Exception* RESTServerException::clone() const
{
	return new RESTServerException(*this);
}


void RESTServerException::rethrow() const
{
	throw *this;
}


std::string RESTServerException::error() const
{
	if (_pContent->has(JSONSerializer::JSON_ERROR))
	{
		return (*_pContent)[JSONSerializer::JSON_ERROR]->stringValue();
	}
	else return ""s;
}


std::string RESTServerException::detail() const
{
	if (_pContent->has(JSONSerializer::JSON_DETAIL))
	{
		return (*_pContent)[JSONSerializer::JSON_DETAIL]->stringValue();
	}
	else return ""s;
}


} } } // namespace Poco::RemotingNG::REST
