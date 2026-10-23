//
// HTTPSClientSession.cpp
//
// Library: NetSSL_OpenSSL
// Package: HTTPSClient
// Module:  HTTPSClientSession
//
// Copyright (c) 2006-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPSSessionInstantiator.h"
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


namespace Poco::Net {


HTTPSClientSession::HTTPSClientSession():
	HTTPClientSession(SecureStreamSocket()),
	_pContext(SSLManager::instance().defaultClientContext())
{
	setPort(HTTPS_PORT);
	initProxySessionFactory();
}


HTTPSClientSession::HTTPSClientSession(const SecureStreamSocket& socket, const std::string& host, Poco::UInt16 port):
	HTTPClientSession(socket),
	_pContext(socket.context())
{
	setHost(host);
	setPort(port);
	initProxySessionFactory();
}


HTTPSClientSession::HTTPSClientSession(const SecureStreamSocket& socket, Session::Ptr pSession):
	HTTPClientSession(socket),
	_pContext(socket.context()),
	_pSession(pSession)
{
	setPort(HTTPS_PORT);
	initProxySessionFactory();
}


HTTPSClientSession::HTTPSClientSession(const std::string& host, Poco::UInt16 port):
	HTTPClientSession(SecureStreamSocket()),
	_pContext(SSLManager::instance().defaultClientContext())
{
	setHost(host);
	setPort(port);
	initProxySessionFactory();
}


HTTPSClientSession::HTTPSClientSession(Context::Ptr pContext):
	HTTPClientSession(SecureStreamSocket(pContext)),
	_pContext(pContext)
{
	initProxySessionFactory(pContext);
}


HTTPSClientSession::HTTPSClientSession(Context::Ptr pContext, Session::Ptr pSession):
	HTTPClientSession(SecureStreamSocket(pContext, pSession)),
	_pContext(pContext),
	_pSession(pSession)
{
	initProxySessionFactory(pContext);
}


HTTPSClientSession::HTTPSClientSession(const std::string& host, Poco::UInt16 port, Context::Ptr pContext):
	HTTPClientSession(SecureStreamSocket(pContext)),
	_pContext(pContext)
{
	setHost(host);
	setPort(port);
	initProxySessionFactory(pContext);
}


HTTPSClientSession::HTTPSClientSession(const std::string& host, Poco::UInt16 port, Context::Ptr pContext, Session::Ptr pSession):
	HTTPClientSession(SecureStreamSocket(pContext, pSession)),
	_pContext(pContext),
	_pSession(pSession)
{
	setHost(host);
	setPort(port);
	initProxySessionFactory(pContext);
}


HTTPSClientSession::~HTTPSClientSession()
{
	try
	{
		_proxySessionFactory.unregisterProtocol("https");
	}
	catch (...)
	{
	}
}


bool HTTPSClientSession::secure() const
{
	return true;
}


void HTTPSClientSession::abort()
{
	SecureStreamSocket sss(socket());
	sss.abort();
}


X509Certificate HTTPSClientSession::serverCertificate()
{
	SecureStreamSocket sss(socket());
	return sss.peerCertificate();
}


std::string HTTPSClientSession::proxyRequestPrefix() const
{
	if (isProxyTunnel())
		return std::string();

	std::string result("https://");
	result.append(getHost());
	// Do not append default port, since this may break some servers.
	// One example of such server is GCS (Google Cloud Storage).
	if (getPort() != HTTPS_PORT)
	{
		result.append(":");
		NumberFormatter::append(result, getPort());
	}
	return result;
}


void HTTPSClientSession::proxyAuthenticate(HTTPRequest& request)
{
	if (!isProxyTunnel())
	{
		proxyAuthenticateImpl(request, getProxyConfig());
	}
}


void HTTPSClientSession::connect(const SocketAddress& address)
{
	bool useProxy = !getProxyHost().empty() && !bypassProxy();

	if (useProxy && isProxyTunnel())
	{
		StreamSocket proxySocket(proxyConnect());
		if (!_sessionHostName.empty() && _sessionHostName != getHost())
		{
			_pSession = nullptr;
		}
		_sessionHostName = getHost();
		SecureStreamSocket secureSocket = SecureStreamSocket::attach(proxySocket, getHost(), _pContext, _pSession, true);
		attachSocket(secureSocket);
		_peerHostName = getHost();
		if (_pContext->sessionCacheEnabled())
		{
			_pSession = secureSocket.currentSession();
		}
	}
	else
	{
		SecureStreamSocket sss(socket());
		// A peer host name that the caller set on the socket is kept.
		if (sss.getPeerHostName().empty() || sss.getPeerHostName() == _peerHostName)
		{
			_peerHostName = useProxy ? getProxyHost() : getHost();
			sss.setPeerHostName(_peerHostName);
		}
		if (!_sessionHostName.empty() && _sessionHostName != sss.getPeerHostName())
		{
			_pSession = nullptr;
			// Without session caching the socket still holds a session given to the constructor.
			sss.useSession(nullptr);
		}
		if (_pContext->sessionCacheEnabled())
		{
			sss.useSession(_pSession);
		}
		_sessionHostName = sss.getPeerHostName();
		HTTPSession::connect(address);
		if (_pContext->sessionCacheEnabled())
		{
			_pSession = sss.currentSession();
		}
	}
}


int HTTPSClientSession::get()
{
	try
	{
		return HTTPSession::get();
	}
	catch (SSLConnectionUnexpectedlyClosedException&)
	{
		return std::char_traits<char>::eof();
	}
}


int HTTPSClientSession::read(char* buffer, std::streamsize length)
{
	try
	{
		return HTTPSession::read(buffer, length);
	}
	catch (SSLConnectionUnexpectedlyClosedException&)
	{
		return 0;
	}
}


Session::Ptr HTTPSClientSession::sslSession()
{
	return _pSession;
}


void HTTPSClientSession::initProxySessionFactory()
{
	_proxySessionFactory.registerProtocol("https", new HTTPSSessionInstantiator);
}


void HTTPSClientSession::initProxySessionFactory(Context::Ptr pContext)
{
	_proxySessionFactory.registerProtocol("https", new HTTPSSessionInstantiator(pContext));
}


} // namespace Poco::Net
