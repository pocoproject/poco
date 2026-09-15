//
// CookieStore.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  CookieStore
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/CookieStore.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


CookieStore::CookieStore()
{
}

	
CookieStore::~CookieStore()
{
}


void CookieStore::addCookies(Poco::Net::HTTPRequest& request)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	Poco::Timestamp now;
	Poco::Net::NameValueCollection cookies;
	for (CookieMap::const_iterator it = _cookies.begin(); it != _cookies.end(); ++it)
	{
		if (it->second.expireTime == 0 || now <= it->second.expireTime)
		{
			cookies.add(it->second.cookie.getName(), it->second.cookie.getValue());
		}
	}
	if (!cookies.empty())
	{
		request.setCookies(cookies);
	}
}


void CookieStore::updateCookies(const Poco::Net::HTTPResponse& response)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	std::vector<Poco::Net::HTTPCookie> cookies;
	response.getCookies(cookies);
	Poco::Timestamp now;
	for (std::vector<Poco::Net::HTTPCookie>::const_iterator it = cookies.begin(); it != cookies.end(); ++it)
	{
		if (it->getMaxAge() == -1)
		{
			CookieInfo ci;
			ci.cookie = *it;
			ci.expireTime = 0;
			_cookies[it->getName()] = ci;
		}
		else if (it->getMaxAge() == 0)
		{
			_cookies.erase(it->getName());
		}
		else
		{
			CookieInfo ci;
			ci.cookie = *it;
			ci.expireTime += it->getMaxAge()*Poco::Timestamp::resolution();
			_cookies[it->getName()] = ci;
		}	
	}
}


} } } // namespace Poco::RemotingNG::REST
