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
#include "Poco/Net/HTTPSessionFactory.h"
#include "Poco/Net/HTTPSessionInstantiator.h"
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
#include "Poco/String.h"
#include <algorithm>
#include <sstream>


using Poco::NumberFormatter;
using Poco::IllegalStateException;
using Poco::InvalidArgumentException;


namespace Poco::Net {


namespace {


bool isConnectionBoundCredentials(const std::string& credentials)
{
	const std::string::size_type begin = credentials.find_first_not_of(" \t");
	if (begin == std::string::npos) return false;
	const std::string::size_type end = credentials.find_first_of(" \t", begin);
	const std::string::size_type length = (end == std::string::npos ? credentials.size() : end) - begin;
	return Poco::icompare(credentials, begin, length, "NTLM") == 0 ||
		Poco::icompare(credentials, begin, length, "Negotiate") == 0 ||
		Poco::icompare(credentials, begin, length, "Kerberos") == 0;
}


bool sendsConnectionBoundCredentials(const HTTPRequest& request)
{
	// A request can carry several credential fields, and any one of them can authenticate the connection.
	return std::any_of(request.begin(), request.end(), [](const auto& field)
	{
		return (Poco::icompare(field.first, HTTPRequest::AUTHORIZATION) == 0 || Poco::icompare(field.first, HTTPRequest::PROXY_AUTHORIZATION) == 0) &&
			isConnectionBoundCredentials(field.second);
	});
}


} // namespace


ProxyConfig HTTPClientSession::_globalProxyConfig;


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
	initProxySessionFactory();
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
	initProxySessionFactory();
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
	initProxySessionFactory();
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
	initProxySessionFactory();
}


HTTPClientSession::HTTPClientSession(const std::string& host, Poco::UInt16 port, const ProxyConfig& proxyConfig):
	_host(host),
	_port(port),
	_sourceAddress4(IPAddress::wildcard(IPAddress::IPv4), 0),
	_sourceAddress6(IPAddress::wildcard(IPAddress::IPv6), 0),
	_proxyConfig(proxyConfig),
	_keepAliveTimeout(DEFAULT_KEEP_ALIVE_TIMEOUT, 0),
	_reconnect(false),
	_mustReconnect(false),
	_expectResponseBody(false),
	_responseReceived(false),
	_ntlmProxyAuthenticated(false)
{
	initProxySessionFactory();
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
	_responseReceived(false),
	_ntlmProxyAuthenticated(false)
{
	initProxySessionFactory();
}


HTTPClientSession::~HTTPClientSession()
{
	try
	{
		_proxySessionFactory.unregisterProtocol("http");
	}
	catch (...)
	{
	}
}


void HTTPClientSession::initProxySessionFactory()
{
	_proxySessionFactory.registerProtocol("http", new HTTPSessionInstantiator);
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


void HTTPClientSession::setProxy(const std::string& host, Poco::UInt16 port, const std::string& protocol, bool tunnel)
{
	if (protocol != "http" && protocol != "https")
		throw InvalidArgumentException("Protocol must be either http or https");
	if (protocol == "https" && !secure())
		throw InvalidArgumentException("HTTPS proxy requires HTTPSClientSession");

	if (!connected())
	{
		_proxyConfig.host = host;
		_proxyConfig.port = port;
		_proxyConfig.protocol = protocol;
		_proxyConfig.tunnel = tunnel;
	}
	else throw IllegalStateException("Cannot set the proxy host, port and protocol for an already connected session");
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


void HTTPClientSession::setProxyProtocol(const std::string& protocol)
{
	if (protocol != "http" && protocol != "https")
		throw InvalidArgumentException("Protocol must be either http or https");
	if (protocol == "https" && !secure())
		throw InvalidArgumentException("HTTPS proxy requires HTTPSClientSession");

	if (!connected())
		_proxyConfig.protocol = protocol;
	else
		throw IllegalStateException("Cannot set the proxy protocol for an already connected session");
}


void HTTPClientSession::setProxyTunnel(bool tunnel)
{
	if (!connected())
		_proxyConfig.tunnel = tunnel;
	else
		throw IllegalStateException("Cannot set the proxy tunnel for an already connected session");
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
	if (config.protocol != "http" && config.protocol != "https")
		throw InvalidArgumentException("Protocol must be either http or https");
	if (config.protocol == "https" && !secure())
		throw InvalidArgumentException("HTTPS proxy requires HTTPSClientSession");

	if (!connected())
		_proxyConfig = config;
	else
		throw IllegalStateException("Cannot set the proxy configuration for an already connected session");
}


void HTTPClientSession::setGlobalProxyConfig(const ProxyConfig& config)
{
	if (config.protocol != "http" && config.protocol != "https")
		throw InvalidArgumentException("Protocol must be either http or https");

	_globalProxyConfig = config;
}


void HTTPClientSession::setKeepAliveTimeout(const Poco::Timespan& timeout)
{
	_keepAliveTimeout = timeout;
}


std::ostream& HTTPClientSession::sendRequest(HTTPRequest& request)
{
	request.validate();
	bool keepAlive = getKeepAlive();
	// Unread bytes of the last response, or data the server sent after it, would be parsed as the next response.
	bool responseDataUnread = keepAlive && connected() && networkException() == nullptr && _pResponseStream != nullptr && (!responseStreamComplete() || dataPending());
	if (responseDataUnread)
		_mustReconnect = true;
	if (_pRequestStream != nullptr)
		_requestBodyComplete = requestStreamComplete();
	bool requestBodyComplete = _requestBodyComplete;
	_requestBodyComplete = true;
	// A failed send or receive or an incomplete request body leaves the connection in an unknown state.
	// Closed before the request stream is released, which would send its buffer again.
	if (connected() && (networkException() != nullptr || !requestBodyComplete))
		close();
	_pRequestStream = nullptr;
	_pResponseStream = nullptr;

	if (_host.empty())
	{
		// A session without a host cannot set up a new connection.
		if (networkException() != nullptr)
			networkException()->rethrow();
		if (!requestBodyComplete)
			throw NetException("Cannot reconnect without a host", "the last request body was not sent to its end");
		if (responseDataUnread)
		{
			close();
			throw NetException("Cannot reconnect without a host", "the last response body was not read to its end, or the server sent data or closed the connection after it");
		}
	}
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
			_connectionAuthenticated = false;
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
			// CodeQL [cpp/auth-bypass]: proxy prefix rewriting, not authentication logic
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
	if (sendsConnectionBoundCredentials(request))
		_connectionAuthenticated = true;
	_pRequestStream = nullptr;
	_pResponseStream = nullptr;
	clearException();
	_responseReceived = false;
	_expectResponseBody = request.getMethod() != HTTPRequest::HTTP_HEAD;
	_connectRequest = request.getMethod() == HTTPRequest::HTTP_CONNECT;
	if (request.getChunkedTransferEncoding())
	{
		{
			// The destructor flushes the header and swallows a write error, which HTTPSession::write() records.
			HTTPHeaderOutputStream hos(*this);
			request.write(hos);
		}
		if (networkException() != nullptr) networkException()->rethrow();
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
	else 
	{
		Poco::CountingOutputStream cs;
		request.write(cs);
		_pRequestStream = new HTTPFixedLengthOutputStream(*this, cs.chars());
		request.write(*_pRequestStream);
	}
	_lastRequest.update();
	return *_pRequestStream;
}


void HTTPClientSession::flushRequest()
{
	if (_pRequestStream != nullptr)
		_requestBodyComplete = requestStreamComplete();
	// Closed before the request stream is released, which would send its buffer on the failed connection.
	if (networkException() != nullptr) close();
	_pRequestStream = nullptr;
	if (networkException() != nullptr) networkException()->rethrow();
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
				// The header parser takes the end of the stream for the end of the header,
				// so a header cut by a failed receive can look complete.
				if (networkException() != nullptr) networkException()->rethrow();
			}
			catch (Exception&)
			{
				close();
				if (networkException() != nullptr)
					networkException()->rethrow();
				else
					throw;
			}
		}
		while (response.getStatus() == HTTPResponse::HTTP_CONTINUE);
	}

	// RFC 9112, 6.3: the framing headers of a response without a body, and of a 2xx response to CONNECT, are ignored.
	const bool tunnelEstablished = _connectRequest && response.getStatus() >= 200 && response.getStatus() < 300;
	// After a 101 response or an established tunnel, the connection no longer carries HTTP messages of this session.
	_mustReconnect = (getKeepAlive() && !response.getKeepAlive()) || response.getStatus() == HTTPResponse::HTTP_SWITCHING_PROTOCOLS || tunnelEstablished;
	const bool hasBody = _expectResponseBody && !tunnelEstablished && response.getStatus() >= 200 && response.getStatus() != HTTPResponse::HTTP_NO_CONTENT && response.getStatus() != HTTPResponse::HTTP_NOT_MODIFIED;
	if (hasBody)
	{
		try
		{
			response.checkFramingHeaders();
		}
		catch (MessageException&)
		{
			close();
			throw;
		}
	}

	if (!hasBody)
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


bool HTTPClientSession::responseComplete() const
{
	if (_pResponseStream == nullptr) return false;
	const auto* pChunked = dynamic_cast<const HTTPChunkedStreamBuf*>(_pResponseStream->rdbuf());
	if (pChunked != nullptr) return pChunked->isComplete();
	const auto* pFixed = dynamic_cast<const HTTPFixedLengthStreamBuf*>(_pResponseStream->rdbuf());
	if (pFixed != nullptr) return pFixed->isComplete();
	return false;
}


bool HTTPClientSession::peekResponse(HTTPResponse& response)
{
	poco_assert (!_responseReceived);

	_pRequestStream->flush();

	if (networkException() != nullptr)
	{
		close();
		networkException()->rethrow();
	}

	response.clear();
	HTTPHeaderInputStream his(*this);
	try
	{
		response.read(his);
		// The header parser takes the end of the stream for the end of the header,
		// so a header cut by a failed receive can look complete.
		if (networkException() != nullptr) networkException()->rethrow();
	}
	catch (Exception&)
	{
		close();
		if (networkException() != nullptr)
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


bool HTTPClientSession::canReuse()
{
	// Data without a pending response means the server has closed the connection or sent stray data.
	return stateAllowsReuse() && !dataPending();
}


bool HTTPClientSession::stateAllowsReuse()
{
	if (!getKeepAlive() || !connected() || networkException() != nullptr) return false;
	if (_pRequestStream != nullptr || !_requestBodyComplete || _pResponseStream == nullptr || mustReconnect()) return false;
	return responseStreamComplete() && buffered() == 0;
}


bool HTTPClientSession::secure() const
{
	return false;
}


int HTTPClientSession::write(const char* buffer, std::streamsize length)
{
	// A request stream flushed on a closed connection would replace the recorded cause with the error of this write.
	if (!connected() && networkException() != nullptr) networkException()->rethrow();
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
			// One attempt per request: after a failed reconnect, a later write must not send body data on a new connection.
			_reconnect = false;
			close();
			try
			{
				reconnect();
				int rc = HTTPSession::write(buffer, length);
				clearException();
				return rc;
			}
			catch (Exception&)
			{
				// A failed connect leaves a new, unconnected socket open; the check at the top needs it closed.
				close();
				throw;
			}
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
	{
		if (_proxyConfig.protocol == "https" && !secure())
			throw Poco::NotImplementedException("HTTPS proxy requires HTTPSClientSession");
		addr = SocketAddress(_proxyConfig.host, _proxyConfig.port);
	}

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
	// Do not append default port, since this may break some servers.
	// One example of such server is GCS (Google Cloud Storage).
	if (_port != HTTPSession::HTTP_PORT)
	{
		result.append(":");
		NumberFormatter::append(result, _port);
	}
	return result;
}


bool HTTPClientSession::requestStreamComplete()
{
	if (networkException() != nullptr) return false;
	try
	{
		auto* pChunked = dynamic_cast<HTTPChunkedOutputStream*>(_pRequestStream.get());
		if (pChunked != nullptr)
		{
			pChunked->rdbuf()->close();
			return true;
		}
		auto* pFixed = dynamic_cast<HTTPFixedLengthOutputStream*>(_pRequestStream.get());
		return pFixed != nullptr && pFixed->rdbuf()->pubsync() == 0 && pFixed->rdbuf()->isComplete();
	}
	catch (Exception&)
	{
		return false;
	}
}


bool HTTPClientSession::responseStreamComplete()
{
	auto* pChunked = dynamic_cast<HTTPChunkedStreamBuf*>(_pResponseStream->rdbuf());
	if (pChunked != nullptr) return pChunked->consumeTerminator();
	auto* pFixed = dynamic_cast<HTTPFixedLengthStreamBuf*>(_pResponseStream->rdbuf());
	if (pFixed != nullptr) return pFixed->skipBuffered();
	return false;
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
	case ProxyAuthentication::None:
		break;

	case ProxyAuthentication::Basic:
		_proxyBasicCreds.setUsername(proxyConfig.username);
		_proxyBasicCreds.setPassword(proxyConfig.password);
		_proxyBasicCreds.proxyAuthenticate(request);
		break;

	case ProxyAuthentication::Digest:
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

	case ProxyAuthentication::NTLM:
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
	if (getProxyProtocol() == "https" && !secure())
		throw Poco::NotImplementedException("HTTPS proxy requires HTTPSClientSession");

	URI proxyUri;
	proxyUri.setScheme(getProxyProtocol());
	proxyUri.setHost(getProxyHost());
	proxyUri.setPort(getProxyPort());

	SharedPtr<HTTPClientSession> proxySession (_proxySessionFactory.createClientSession(proxyUri));

	proxySession->setTimeout(getTimeout());
	// RFC 3986 requires IPv6 literal hosts to be bracketed in the authority form
	// used as the CONNECT request-target and in the Host header.
	std::string targetAddress;
	if (_host.find(':') != std::string::npos)
		targetAddress.append("[").append(_host).append("]");
	else
		targetAddress.append(_host);
	targetAddress.append(":");
	NumberFormatter::append(targetAddress, _port);
	HTTPRequest proxyRequest(HTTPRequest::HTTP_CONNECT, targetAddress, HTTPMessage::HTTP_1_1);
	HTTPResponse proxyResponse;
	proxyRequest.set(HTTPMessage::PROXY_CONNECTION, HTTPMessage::CONNECTION_KEEP_ALIVE);
	proxyRequest.set(HTTPRequest::HOST, targetAddress);
	proxySession->proxyAuthenticateImpl(proxyRequest, _proxyConfig);
	proxySession->setKeepAlive(true);
	proxySession->setSourceAddress(_sourceAddress4);
	proxySession->setSourceAddress(_sourceAddress6);
	proxySession->sendRequest(proxyRequest);
	proxySession->receiveResponse(proxyResponse);
	if (proxyResponse.getStatus() != HTTPResponse::HTTP_OK)
		throw HTTPException("Cannot establish proxy connection (" + std::to_string(proxyResponse.getStatus()) + ")", proxyResponse.getReason());
	return proxySession->detachSocket();
}


void HTTPClientSession::proxyTunnel()
{
	StreamSocket ss = proxyConnect();
	attachSocket(ss);
	// A response of the previous connection must not decide whether the tunnel is reused.
	_pResponseStream = nullptr;
	_mustReconnect = false;
}


bool HTTPClientSession::bypassProxy() const
{
	if (!_proxyConfig.nonProxyHosts.empty())
	{
		return RegularExpression::match(_host, _proxyConfig.nonProxyHosts, RegularExpression::RE_CASELESS | RegularExpression::RE_ANCHORED);
	}
	else return false;
}

} // namespace Poco::Net
