//
// Tester.cpp
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Tester.h"
#include "Poco/Exception.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPCookie.h"


Object Tester::put(int id, const Object& object)
{
	Object result;
	switch (id)
	{
	case ID_SET_COOKIE:
		setCookie();
		break;
	case ID_VERIFY_COOKIE:
		result.data = verifyCookie() ? "OK" : "FAIL";
		break;
	case ID_FAULT:
		throw Poco::ApplicationException("Something went wrong");
	default:
		_data[id] = object.data;
		result = object;
		break;
	}
	return result;
}


Object Tester::post(int id, const Object& object)
{
	_data[id] = object.data;
	return object;
}


Object Tester::get(int id, const std::string& options)
{
	std::map<int, std::string>::const_iterator it = _data.find(id);
	if (it != _data.end())
	{
		Object result;
		result.data = it->second;
		return result;
	}
	else throw Poco::NotFoundException();
}


void Tester::delete_(int id)
{
	std::map<int, std::string>::iterator it = _data.find(id);
	if (it != _data.end())
	{
		_data.erase(it);
	}
	else throw Poco::NotFoundException();
}


void Tester::setCookie()
{
	Poco::RemotingNG::Context::Ptr pContext = Poco::RemotingNG::Context::get();
	Poco::Net::HTTPServerResponse* pResponse = pContext->getValue<Poco::Net::HTTPServerResponse*>("httpResponse");
	Poco::Net::HTTPCookie cookie("testcookie", "testvalue");
	cookie.setMaxAge(5);
	pResponse->addCookie(cookie);
}


bool Tester::verifyCookie()
{
	Poco::RemotingNG::Context::Ptr pContext = Poco::RemotingNG::Context::get();
	Poco::Net::HTTPServerRequest* pRequest = pContext->getValue<Poco::Net::HTTPServerRequest*>("httpRequest");
	Poco::Net::NameValueCollection cookies;
	pRequest->getCookies(cookies);
	return cookies.get("testcookie", "") == "testvalue";
}


Object AuthTester::post(int id, const Object& object)
{
	return object;
}


Object AuthTester::get(int id, const std::string& options)
{
	Object result;
	return result;
}
