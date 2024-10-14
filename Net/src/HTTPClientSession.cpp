//
// HTTPClientSession.cpp
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPClientSession
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPHeaderStream.h"
#include "Poco/Net/HTTPStream.h"
#include "Poco/Net/HTTPFixedLengthStream.h"
#include "Poco/Net/HTTPChunkedStream.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/NetException.h"
#include "Poco/NumberFormatter.h"
#include "Poco/CountingStream.h"
#include "Poco/RegularExpression.h"
#include <sstream>


using Poco::NumberFormatter;
using Poco::IllegalStateException;


namespace Poco {
namespace Net {


HTTPClientSession::ProxyConfig HTTPClientSession::_globalProxyConfig;


HTTPClientSession::HTTPClientSession():
	_port(HTTPSession::HTTP_PORT),
	_sourceAddress4(IPAddress::wildcard(IPAddress::IPv4), 0),
	_sourceAddress6(IPAddress::wildcard(IPAddress::IPv6), 0),
	_proxyConfig(_globalProxyConfig),
	_keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
	_reconnect(false),
	_mustReconnect(false),
	_expectResponseBody(false),
	_responseReceived(false),
	_ntlmProxyAuthenticated(false)
{
}


HTTPClientSession::HTTPClientSession(const StreamSocket& socket):
	HTTPSession(socket),
	_port(HTTPSession::HTTP_PORT),
	_sourceAddress4(IPAddress::wildcard(IPAddress::IPv4), 0),
	_sourceAddress6(IPAddress::wildcard(IPAddress::IPv6), 0),
	_proxyConfig(_globalProxyConfig),
	_keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
	_reconnect(false),
	_mustReconnect(false),
	_expectResponseBody(false),
	_responseReceived(false),
	_ntlmProxyAuthenticated(false)
{
}


HTTPClientSession::HTTPClientSession(const SocketAddress& address):
	_host(address.host().toString()),
	_port(address.port()),
	_sourceAddress4(IPAddress::wildcard(IPAddress::IPv4), 0),
	_sourceAddress6(IPAddress::wildcard(IPAddress::IPv6), 0),
	_proxyConfig(_globalProxyConfig),
	_keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
	_reconnect(false),
	_mustReconnect(false),
	_expectResponseBody(false),
	_responseReceived(false),
	_ntlmProxyAuthenticated(false)
{
}


HTTPClientSession::HTTPClientSession(const std::string& host, Poco::UInt16 port):
	_host(host),
	_port(port),
	_sourceAddress4(IPAddress::wildcard(IPAddress::IPv4), 0),
	_sourceAddress6(IPAddress::wildcard(IPAddress::IPv6), 0),
	_proxyConfig(_globalProxyConfig),
	_keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
	_reconnect(false),
	_mustReconnect(false),
	_expectResponseBody(false),
	_responseReceived(false),
	_ntlmProxyAuthenticated(false)
{
}


HTTPClientSession::HTTPClientSession(const std::string& host, Poco::UInt16 port, const ProxyConfig& proxyConfig):
	_host(host),
	_port(port),
	_proxyConfig(proxyConfig),
	_keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
	_reconnect(false),
	_mustReconnect(false),
	_expectResponseBody(false),
	_responseReceived(false),
	_ntlmProxyAuthenticated(false)
{
}


HTTPClientSession::HTTPClientSession(const StreamSocket& socket, const ProxyConfig& proxyConfig):
	HTTPSession(socket),
	_port(HTTPSession::HTTP_PORT),
	_sourceAddress4(IPAddress::wildcard(IPAddress::IPv4), 0),
	_sourceAddress6(IPAddress::wildcard(IPAddress::IPv6), 0),
	_proxyConfig(proxyConfig),
	_keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
	_reconnect(false),
	_mustReconnect(false),
	_expectResponseBody(false),
	_responseReceived(false)
{
}


HTTPClientSession::~HTTPClientSession()
{
}


void HTTPClientSession::setHost(const std::string& host)
{
	if (!connected())
		_host = host;
	else
		throw IllegalStateException("Cannot set the host for an already connected session");
}


void HTTPClientSession::setPort(Poco::UInt16 port)
{
	if (!connected())
		_port = port;
	else
		throw IllegalStateException("Cannot set the port number for an already connected session");
}


void HTTPClientSession::setSourceAddress(const SocketAddress& address)
{
	if (!connected())
	{
		if (address.family() == IPAddress::IPv4)
			_sourceAddress4 = address;
		else
			_sourceAddress6 = address;
		_sourceAddress = address;
	}
	else
		throw IllegalStateException("Cannot set the source address for an already connected session");
}


const SocketAddress& HTTPClientSession::getSourceAddress()
{
	return _sourceAddress;
}


const SocketAddress& HTTPClientSession::getSourceAddress4()
{
	return _sourceAddress4;
}


const SocketAddress& HTTPClientSession::getSourceAddress6()
{
	return _sourceAddress6;
}


void HTTPClientSession::setProxy(const std::string& host, Poco::UInt16 port)
{
	if (!connected())
	{
		_proxyConfig.host = host;
		_proxyConfig.port = port;
	}
	else throw IllegalStateException("Cannot set the proxy host and port for an already connected session");
}


void HTTPClientSession::setProxyHost(const std::string& host)
{
	if (!connected())
		_proxyConfig.host = host;
	else
		throw IllegalStateException("Cannot set the proxy host for an already connected session");
}


void HTTPClientSession::setProxyPort(Poco::UInt16 port)
{
	if (!connected())
		_proxyConfig.port = port;
	else
		throw IllegalStateException("Cannot set the proxy port number for an already connected session");
}


void HTTPClientSession::setProxyCredentials(const std::string& username, const std::string& password)
{
	_proxyConfig.username = username;
	_proxyConfig.password = password;
}


void HTTPClientSession::setProxyUsername(const std::string& username)
{
	_proxyConfig.username = username;
}


void HTTPClientSession::setProxyPassword(const std::string& password)
{
	_proxyConfig.password = password;
}


void HTTPClientSession::setProxyConfig(const ProxyConfig& config)
{
	_proxyConfig = config;
}


void HTTPClientSession::setGlobalProxyConfig(const ProxyConfig& config)
{
	_globalProxyConfig = config;
}


void HTTPClientSession::setKeepAliveTimeout(const Poco::Timespan& timeout)
{
	_keepAliveTimeout = timeout;
}


std::ostream& HTTPClientSession::sendRequest(HTTPRequest& request)
{
	_pRequestStream = 0;
	_pResponseStream = 0;

	bool keepAlive = getKeepAlive();
	if (((connected() && !keepAlive) || mustReconnect()) && !_host.empty())
	{
		close();
		_mustReconnect = false;
	}
	try
	{
		if (!connected())
		{
			_ntlmProxyAuthenticated = false;
			reconnect();
		}
		if (!keepAlive)
		{
			request.setKeepAlive(false);
		}
		if (!request.has(HTTPRequest::HOST) && !_host.empty())
		{
			request.setHost(_host, _port);
		}
		if (!_proxyConfig.host.empty() && !bypassProxy())
		{
			std::string prefix = proxyRequestPrefix();
			if (!prefix.empty() && request.getURI().compare(0, 7, "http://") != 0 && request.getURI().compare(0, 8, "https://") != 0)
				request.setURI(prefix + request.getURI());
			if (keepAlive) request.set(HTTPMessage::PROXY_CONNECTION, HTTPMessage::CONNECTION_KEEP_ALIVE);
			proxyAuthenticate(request);
		}
		_reconnect = keepAlive;
		return sendRequestImpl(request);
	}
	catch (Exception&)
	{
		close();
		throw;
	}
}


std::ostream& HTTPClientSession::sendRequestImpl(const HTTPRequest& request)
{
	_pRequestStream = 0;
	_pResponseStream = 0;
	clearException();
	_responseReceived = false;
	_expectResponseBody = request.getMethod() != HTTPRequest::HTTP_HEAD;
	const std::string& method = request.getMethod();
	if (request.getChunkedTransferEncoding())
	{
		HTTPHeaderOutputStream hos(*this);
		request.write(hos);
		_pRequestStream = new HTTPChunkedOutputStream(*this, &requestTrailer());
	}
	else if (request.hasContentLength())
	{
		Poco::CountingOutputStream cs;
		request.write(cs);
#if POCO_HAVE_INT64
		_pRequestStream = new HTTPFixedLengthOutputStream(*this, request.getContentLength64() + cs.chars());
#else
		_pRequestStream = new HTTPFixedLengthOutputStream(*this, request.getContentLength() + cs.chars());
#endif
		request.write(*_pRequestStream);
	}
	else if ((method != HTTPRequest::HTTP_PUT && method != HTTPRequest::HTTP_POST && method != HTTPRequest::HTTP_PATCH) || request.has(HTTPRequest::UPGRADE))
	{
		Poco::CountingOutputStream cs;
		request.write(cs);
		_pRequestStream = new HTTPFixedLengthOutputStream(*this, cs.chars());
		request.write(*_pRequestStream);
	}
	else
	{
		_pRequestStream = new HTTPOutputStream(*this);
		request.write(*_pRequestStream);
	}
	_lastRequest.update();
	return *_pRequestStream;
}


void HTTPClientSession::flushRequest()
{
	_pRequestStream = 0;
	if (networkException()) networkException()->rethrow();
}


std::istream& HTTPClientSession::receiveResponse(HTTPResponse& response)
{
	flushRequest();
	responseTrailer().clear();
	if (!_responseReceived)
	{
		do
		{
			response.clear();
			HTTPHeaderInputStream his(*this);
			try
			{
				response.read(his);
			}
			catch (Exception&)
			{
				close();
				if (networkException())
					networkException()->rethrow();
				else
					throw;
			}
		}
		while (response.getStatus() == HTTPResponse::HTTP_CONTINUE);
	}

	_mustReconnect = getKeepAlive() && !response.getKeepAlive();

	if (!_expectResponseBody || response.getStatus() < 200 || response.getStatus() == HTTPResponse::HTTP_NO_CONTENT || response.getStatus() == HTTPResponse::HTTP_NOT_MODIFIED)
		_pResponseStream = new HTTPFixedLengthInputStream(*this, 0);
	else if (response.getChunkedTransferEncoding())
		_pResponseStream = new HTTPChunkedInputStream(*this, &responseTrailer());
	else if (response.hasContentLength())
#if defined(POCO_HAVE_INT64)
		_pResponseStream = new HTTPFixedLengthInputStream(*this, response.getContentLength64());
#else
		_pResponseStream = new HTTPFixedLengthInputStream(*this, response.getContentLength());
#endif
	else
		_pResponseStream = new HTTPInputStream(*this);

	return *_pResponseStream;
}


bool HTTPClientSession::peekResponse(HTTPResponse& response)
{
	poco_assert (!_responseReceived);

	_pRequestStream->flush();

	if (networkException()) networkException()->rethrow();

	response.clear();
	HTTPHeaderInputStream his(*this);
	try
	{
		response.read(his);
	}
	catch (Exception&)
	{
		close();
		if (networkException())
			networkException()->rethrow();
		else
			throw;
		throw;
	}
	_responseReceived = response.getStatus() != HTTPResponse::HTTP_CONTINUE;
	return !_responseReceived;
}


void HTTPClientSession::reset()
{
	close();
}


bool HTTPClientSession::secure() const
{
	return false;
}


int HTTPClientSession::write(const char* buffer, std::streamsize length)
{
	try
	{
		int rc = HTTPSession::write(buffer, length);
		_reconnect = false;
		return rc;
	}
	catch (IOException&)
	{
		if (_reconnect)
		{
			close();
			reconnect();
			int rc = HTTPSession::write(buffer, length);
			clearException();
			_reconnect = false;
			return rc;
		}
		else throw;
	}
}


void HTTPClientSession::reconnect()
{
	SocketAddress addr;
	if (_proxyConfig.host.empty() || bypassProxy())
	{
		if (SocketAddress::isUnixLocal(_host))
			addr = SocketAddress(_host);
		else
			addr = SocketAddress(_host, _port);
	}
	else
		addr = SocketAddress(_proxyConfig.host, _proxyConfig.port);

	if ((!_sourceAddress4.host().isWildcard()) || (_sourceAddress4.port() != 0))
		connect(addr, _sourceAddress4);
	else if ((!_sourceAddress6.host().isWildcard()) || (_sourceAddress6.port() != 0))
		connect(addr, _sourceAddress6);
	else
		connect(addr);
}


std::string HTTPClientSession::proxyRequestPrefix() const
{
	std::string result("http://");
	result.append(_host);
	result.append(":");
	NumberFormatter::append(result, _port);
	return result;
}


bool HTTPClientSession::mustReconnect() const
{
	if (!_mustReconnect)
	{
		Poco::Timestamp now;
		return _keepAliveTimeout <= now - _lastRequest;
	}
	else return true;
}


void HTTPClientSession::proxyAuthenticate(HTTPRequest& request)
{
	proxyAuthenticateImpl(request, _proxyConfig);
}


void HTTPClientSession::proxyAuthenticateImpl(HTTPRequest& request, const ProxyConfig& proxyConfig)
{
	switch (proxyConfig.authMethod)
	{
	case PROXY_AUTH_NONE:
		break;

	case PROXY_AUTH_HTTP_BASIC:
		_proxyBasicCreds.setUsername(proxyConfig.username);
		_proxyBasicCreds.setPassword(proxyConfig.password);
		_proxyBasicCreds.proxyAuthenticate(request);
		break;

	case PROXY_AUTH_HTTP_DIGEST:
		if (HTTPCredentials::hasDigestCredentials(request))
		{
			_proxyDigestCreds.updateProxyAuthInfo(request);
		}
		else
		{
			_proxyDigestCreds.setUsername(proxyConfig.username);
			_proxyDigestCreds.setPassword(proxyConfig.password);
			proxyAuthenticateDigest(request);
		}
		break;

	case PROXY_AUTH_NTLM:
		if (_ntlmProxyAuthenticated)
		{
			_proxyNTLMCreds.updateProxyAuthInfo(request);
		}
		else
		{
			_proxyNTLMCreds.setUsername(proxyConfig.username);
			_proxyNTLMCreds.setPassword(proxyConfig.password);
			_proxyNTLMCreds.setHost(proxyConfig.host);
			proxyAuthenticateNTLM(request);
			_ntlmProxyAuthenticated = true;
		}
		break;
	}
}


void HTTPClientSession::proxyAuthenticateDigest(HTTPRequest& request)
{
	HTTPResponse response;
	request.set(HTTPMessage::PROXY_CONNECTION, HTTPMessage::CONNECTION_KEEP_ALIVE);
	sendChallengeRequest(request, response);
	_proxyDigestCreds.proxyAuthenticate(request, response);
}


void HTTPClientSession::proxyAuthenticateNTLM(HTTPRequest& request)
{
	HTTPResponse response;
	request.set(HTTPMessage::PROXY_CONNECTION, HTTPMessage::CONNECTION_KEEP_ALIVE);
	_proxyNTLMCreds.proxyAuthenticate(request, std::string());
	sendChallengeRequest(request, response);
	_proxyNTLMCreds.proxyAuthenticate(request, response);
}


void HTTPClientSession::sendChallengeRequest(const HTTPRequest& request, HTTPResponse& response)
{
	if (!connected())
	{
		reconnect();
	}

	HTTPRequest challengeRequest(request);
	if (challengeRequest.hasContentLength())
	{
		challengeRequest.setContentLength(0);
	}

	sendRequestImpl(challengeRequest);
	std::istream& istr = receiveResponse(response);
	while (istr.good()) istr.get();
}


StreamSocket HTTPClientSession::proxyConnect()
{
	ProxyConfig emptyProxyConfig;
	HTTPClientSession proxySession(getProxyHost(), getProxyPort(), emptyProxyConfig);
	proxySession.setTimeout(getTimeout());
	std::string targetAddress(_host);
	targetAddress.append(":");
	NumberFormatter::append(targetAddress, _port);
	HTTPRequest proxyRequest(HTTPRequest::HTTP_CONNECT, targetAddress, HTTPMessage::HTTP_1_1);
	HTTPResponse proxyResponse;
	proxyRequest.set(HTTPMessage::PROXY_CONNECTION, HTTPMessage::CONNECTION_KEEP_ALIVE);
	proxyRequest.set(HTTPRequest::HOST, getHost());
	proxySession.proxyAuthenticateImpl(proxyRequest, _proxyConfig);
	proxySession.setKeepAlive(true);
	proxySession.setSourceAddress(_sourceAddress4);
	proxySession.setSourceAddress(_sourceAddress6);
	proxySession.sendRequest(proxyRequest);
	proxySession.receiveResponse(proxyResponse);
	if (proxyResponse.getStatus() != HTTPResponse::HTTP_OK)
		throw HTTPException("Cannot establish proxy connection", proxyResponse.getReason());
	return proxySession.detachSocket();
}


void HTTPClientSession::proxyTunnel()
{
	StreamSocket ss = proxyConnect();
	attachSocket(ss);
}


bool HTTPClientSession::bypassProxy() const
{
	if (!_proxyConfig.nonProxyHosts.empty())
	{
		return RegularExpression::match(_host, _proxyConfig.nonProxyHosts, RegularExpression::RE_CASELESS | RegularExpression::RE_ANCHORED);
	}
	else return false;
}


} } // namespace Poco::Net
