//
// HTTPStreamFactory.cpp
//
// Library: Net
// Package: HTTP
// Module:  HTTPStreamFactory
//
// Copyright (c) 2005-2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPIOStream.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/NetException.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/UnbufferedStreamBuf.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Format.h"
#include "Poco/Version.h"


using Poco::URIStreamFactory;
using Poco::URI;
using Poco::URIStreamOpener;
using Poco::UnbufferedStreamBuf;
using namespace std::string_literals;


namespace Poco {
namespace Net {


HTTPStreamFactory::HTTPStreamFactory():
	_proxyPort(HTTPSession::HTTP_PORT)
{
}


HTTPStreamFactory::HTTPStreamFactory(const std::string& proxyHost, Poco::UInt16 proxyPort):
	_proxyHost(proxyHost),
	_proxyPort(proxyPort)
{
}


HTTPStreamFactory::HTTPStreamFactory(const std::string& proxyHost, Poco::UInt16 proxyPort, const std::string& proxyUsername, const std::string& proxyPassword):
	_proxyHost(proxyHost),
	_proxyPort(proxyPort),
	_proxyUsername(proxyUsername),
	_proxyPassword(proxyPassword)
{
}


HTTPStreamFactory::~HTTPStreamFactory()
{
}


std::istream* HTTPStreamFactory::open(const URI& uri)
{
	poco_assert (uri.getScheme() == "http");

	URI resolvedURI(uri);
	URI proxyUri;
	HTTPClientSession* pSession = 0;
	HTTPResponse res;
	bool retry = false;
	bool authorize = false;
	std::string username;
	std::string password;

	try
	{
		do
		{
			if (!pSession)
			{
				pSession = new HTTPClientSession(resolvedURI.getHost(), resolvedURI.getPort());

				if (proxyUri.empty())
				{
					if (!_proxyHost.empty())
					{
						pSession->setProxy(_proxyHost, _proxyPort);
						pSession->setProxyCredentials(_proxyUsername, _proxyPassword);
					}
				}
				else
				{
					pSession->setProxy(proxyUri.getHost(), proxyUri.getPort());
					if (!_proxyUsername.empty())
					{
						pSession->setProxyCredentials(_proxyUsername, _proxyPassword);
					}
				}
			}

			std::string path = resolvedURI.getPathAndQuery();
			if (path.empty()) path = "/";
			HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);

			if (authorize)
			{
				HTTPCredentials::extractCredentials(uri, username, password);
				HTTPCredentials cred(username, password);
				cred.authenticate(req, res);
			}

			req.set("User-Agent"s, Poco::format("poco/%d.%d.%d"s,
				(POCO_VERSION >> 24) & 0xFF,
				(POCO_VERSION >> 16) & 0xFF,
				(POCO_VERSION >> 8) & 0xFF));
			req.set("Accept"s, "*/*"s);

			pSession->sendRequest(req);
			std::istream& rs = pSession->receiveResponse(res);
			bool moved = (res.getStatus() == HTTPResponse::HTTP_MOVED_PERMANENTLY ||
						  res.getStatus() == HTTPResponse::HTTP_FOUND ||
						  res.getStatus() == HTTPResponse::HTTP_SEE_OTHER ||
						  res.getStatus() == HTTPResponse::HTTP_TEMPORARY_REDIRECT);
			if (moved)
			{
				resolvedURI.resolve(res.get("Location"s));
				if (!username.empty())
				{
					resolvedURI.setUserInfo(username + ":" + password);
				}
				throw URIRedirection(resolvedURI.toString());
			}
			else if (res.getStatus() == HTTPResponse::HTTP_OK)
			{
				return new HTTPResponseStream(rs, pSession);
			}
			else if (res.getStatus() == HTTPResponse::HTTP_USE_PROXY && !retry)
			{
				// The requested resource MUST be accessed through the proxy
				// given by the Location field. The Location field gives the
				// URI of the proxy. The recipient is expected to repeat this
				// single request via the proxy. 305 responses MUST only be generated by origin servers.
				// only use for one single request!
				proxyUri.resolve(res.get("Location"s));
				delete pSession;
				pSession = 0;
				retry = true; // only allow useproxy once
			}
			else if (res.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED && !authorize)
			{
				authorize = true;
				retry = true;
				Poco::NullOutputStream null;
				Poco::StreamCopier::copyStream(rs, null);
			}
			else throw HTTPException(res.getReason(), uri.toString());
		}
		while (retry);
		throw HTTPException("Too many redirects", uri.toString());
	}
	catch (...)
	{
		delete pSession;
		throw;
	}
}


void HTTPStreamFactory::registerFactory()
{
	URIStreamOpener::defaultOpener().registerStreamFactory("http"s, new HTTPStreamFactory);
}


void HTTPStreamFactory::unregisterFactory()
{
	URIStreamOpener::defaultOpener().unregisterStreamFactory("http"s);
}


} } // namespace Poco::Net
