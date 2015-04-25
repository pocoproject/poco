//
// HTTPSClient.cpp
//
// $Id: //poco/1.4/NetSSL_OpenSSL/src/HTTPSClient.cpp#4 $
//
// Library: NetSSL_OpenSSL
// Package: HTTPSClient
// Module:  HTTPSClient
//
// Copyright (c) 2006-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPSClient.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureStreamSocketImpl.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/NumberFormatter.h"


using Poco::NumberFormatter;
using Poco::IllegalStateException;


namespace Poco {
namespace Net {


HTTPSClient::HTTPSClient(const URI& uri,
	InstantiatorPtr pInstantiator,
	bool redirect):
	HTTPClient(uri, pInstantiator, redirect)
{
}


HTTPSClient::HTTPSClient(const SocketAddress& address,
	const std::string& scheme,
	InstantiatorPtr pInstantiator,
	bool redirect):
	HTTPClient(address, scheme, pInstantiator, redirect)
{
}


HTTPSClient::HTTPSClient(const std::string& host,
	Poco::UInt16 port,
	const std::string& scheme,
	InstantiatorPtr pInstantiator,
	bool redirect):
	HTTPClient(host, port, scheme, pInstantiator, redirect)
{
}


HTTPSClient::HTTPSClient(const std::string& host,
	Poco::UInt16 port,
	const HTTPClientSession::ProxyConfig& proxyConfig,
	const std::string& scheme,
	InstantiatorPtr pInstantiator,
	bool redirect):
	HTTPClient(host, port, proxyConfig, scheme, pInstantiator, redirect)
{
}


HTTPSClient::~HTTPSClient()
{
}


bool HTTPSClient::secure() const
{
	return true;
}

/*
void HTTPSClient::abort()
{
	SecureStreamSocket sss(socket());
	sss.abort();
}


X509Certificate HTTPSClient::serverCertificate()
{
	SecureStreamSocket sss(socket());
	return sss.peerCertificate();
}


std::string HTTPSClient::proxyRequestPrefix() const
{
	return std::string();
}


void HTTPSClient::proxyAuthenticate(HTTPRequest& request)
{
}


void HTTPSClient::connect(const SocketAddress& address)
{
	if (getProxyHost().empty() || bypassProxy())
	{
		SecureStreamSocket sss(socket());
		if (_pContext->sessionCacheEnabled())
		{
			sss.useSession(_pSession);
		}
		connect(address);
		if (_pContext->sessionCacheEnabled())
		{
			_pSession = sss.currentSession();
		}
	}
	else
	{
		StreamSocket proxySocket(proxyConnect());
		SecureStreamSocket secureSocket = SecureStreamSocket::attach(proxySocket, getHost(), _pContext, _pSession);
		attachSocket(secureSocket);
		if (_pContext->sessionCacheEnabled())
		{
			_pSession = secureSocket.currentSession();
		}
	}
}


int HTTPSClient::read(char* buffer, std::streamsize length)
{
	try
	{
		return read(buffer, length);
	} 
	catch(SSLConnectionUnexpectedlyClosedException&)
	{
		return 0;
	}
}
*/

Session::Ptr HTTPSClient::sslSession()
{
	return _pSession;
}


} } // namespace Poco::Net
