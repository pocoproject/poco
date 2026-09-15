//
// Transport.cpp
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  Transport
//
// Copyright (c) 2006-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/JSONRPC/Transport.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSessionFactory.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/NullStream.h"
#include "Poco/InflatingStream.h"
#include "Poco/DeflatingStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/NumberFormatter.h"


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


const std::string Transport::PROTOCOL("jsonrpc");
const std::string Transport::USER_AGENT("User-Agent");
const std::string Transport::CONTENT_TYPE("application/json; charset=\"utf-8\"");


Poco::Net::HTTPSessionFactory Transport::_httpSessionFactory;


Transport::Transport():
	_keepAlive(false),
	_chunkedTransferEncoding(true),
	_compression(false),
	_authMode(AUTH_NONE),
	_pCookieStore(new CookieStore)
{
}


Transport::~Transport()
{
}


Poco::Timespan Transport::getTimeout() const
{
	return requireSession().getTimeout();
}


void Transport::setTimeout(const Poco::Timespan& timeout)
{
	requireSession().setTimeout(timeout);
}


bool Transport::isKeepAliveEnabled() const
{
	return _keepAlive;
}


void Transport::enableKeepAlive(bool enable)
{
	requireSession().setKeepAlive(enable);
	_keepAlive = enable;
}


Poco::Timespan Transport::getKeepAliveTimeout() const
{
	return requireSession().getKeepAliveTimeout();
}


void Transport::setKeepAliveTimeout(const Poco::Timespan& timeout)
{
	requireSession().setKeepAliveTimeout(timeout);
}


bool Transport::isChunkedTransferEncodingEnabled() const
{
	return _chunkedTransferEncoding;
}


void Transport::enableChunkedTransferEncoding(bool enable)
{
	_chunkedTransferEncoding = enable;
}


bool Transport::isCompressionEnabled() const
{
	return _compression;
}


void Transport::enableCompression(bool enable)
{
	_compression = enable;
}


void Transport::setAuthentication(AuthMode mode)
{
	_authMode = mode;
}


Transport::AuthMode Transport::getAuthentication() const
{
	return _authMode;
}


void Transport::setCredentials(const Credentials& creds)
{
	setUsername(creds.getAttribute(Credentials::ATTR_USERNAME));
	setPassword(creds.getAttribute(Credentials::ATTR_PASSWORD));
}


Credentials Transport::getCredentials() const
{
	Credentials creds;
	creds.setAttribute(Credentials::ATTR_USERNAME, getUsername());
	creds.setAttribute(Credentials::ATTR_PASSWORD, getPassword());
	return creds;
}


void Transport::setUsername(const std::string& username)
{
	_creds.setUsername(username);
}


const std::string& Transport::getUsername() const
{
	return _creds.getUsername();
}


void Transport::setPassword( const std::string& password)
{
	_creds.setPassword(password);
}


const std::string& Transport::getPassword() const
{
	return _creds.getPassword();
}


const std::string& Transport::getUserAgent() const
{
	return _userAgent;
}


void Transport::setUserAgent(const std::string& userAgent)
{
	_userAgent = userAgent;
}


void Transport::setCustomHeader(const std::string& name, const std::string& value)
{
	_customHeaders[name] = value;
}


void Transport::removeCustomHeader(const std::string& name)
{
	_customHeaders.erase(name);	
}


bool Transport::hasCustomHeader(const std::string& name) const
{
	return _customHeaders.find(name) != _customHeaders.end();
}


const std::string& Transport::getCustomHeader(const std::string& name) const
{
	const auto it = _customHeaders.find(name);
	if (it != _customHeaders.end())
		return it->second;
	else
		throw Poco::NotFoundException(name);
}


void Transport::setProxyConfig(const Poco::Net::ProxyConfig& proxyConfig)
{
	_proxyConfig = proxyConfig;
}


const Poco::Net::ProxyConfig& Transport::getProxyConfig() const
{
	return _proxyConfig;
}


CookieStore::Ptr Transport::getCookieStore() const
{
	return _pCookieStore;
}


void Transport::setCookieStore(CookieStore::Ptr pCookieStore)
{
	_pCookieStore = pCookieStore;
}


const std::string& Transport::endPoint() const
{
	return _endPoint;
}


void Transport::connect(const std::string& endPoint)
{
	_endPoint = endPoint;
	_endPointURI = endPoint;
	_pSession = _httpSessionFactory.createClientSession(_endPointURI);
	_pSession->setKeepAlive(_keepAlive);
	if (!_proxyConfig.host.empty())
	{
		_pSession->setProxyConfig(_proxyConfig);
	}
}


void Transport::disconnect()
{
	_pSession = 0;
	_endPoint.clear();
	_endPointURI.clear();
}


bool Transport::connected() const
{
	return !_pSession.isNull();
}


Poco::RemotingNG::Serializer& Transport::beginMessage(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType)
{
	return beginRequest(oid, tid, messageName, messageType);
}


void Transport::sendMessage(const Poco::RemotingNG::Identifiable::ObjectId& /*oid*/, const Poco::RemotingNG::Identifiable::TypeId& /*tid*/, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType /*messageType*/)
{
	_pDeflater = 0;
	if (!_chunkedTransferEncoding)
	{
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, _endPointURI.getPathEtc(), Poco::Net::HTTPMessage::HTTP_1_1);
		prepareRequest(request, messageName);
		request.setContentLength(static_cast<int>(_pStream->tellp()));
		std::ostream& ostr = _pSession->sendRequest(request);
		Poco::StreamCopier::copyStream(*_pStream, ostr);
	}

	Poco::Net::HTTPResponse response;
	std::istream& istr = _pSession->receiveResponse(response);
	if (_pCookieStore)
	{
		_pCookieStore->updateCookies(response);
	}
	Poco::NullOutputStream ostr;
	Poco::StreamCopier::copyStream(istr, ostr);
}


Poco::RemotingNG::Serializer& Transport::beginRequest(const Poco::RemotingNG::Identifiable::ObjectId& /*oid*/, const Poco::RemotingNG::Identifiable::TypeId& /*tid*/, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType)
{
	if (messageType == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST)
	{
		if (_chunkedTransferEncoding)
		{
			Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, _endPointURI.getPathEtc(), Poco::Net::HTTPMessage::HTTP_1_1);
			prepareRequest(request, messageName);
			std::ostream& ostr = _pSession->sendRequest(request);
			if (_compression)
			{
				_pDeflater = new Poco::DeflatingOutputStream(ostr, Poco::DeflatingStreamBuf::STREAM_GZIP, 1);
				_serializer.setup(*_pDeflater);
			}
			else
			{
				_serializer.setup(ostr);
			}
		}
		else
		{
			_pStream = new std::stringstream;
			_serializer.setup(*_pStream);
		}
		return _serializer;
	}
	else
	{
		poco_bugcheck();
		throw Poco::BugcheckException();
	}
}


Poco::RemotingNG::Deserializer& Transport::sendRequest(const Poco::RemotingNG::Identifiable::ObjectId& /*oid*/, const Poco::RemotingNG::Identifiable::TypeId& /*tid*/, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType /*messageType*/)
{
	_pDeflater = 0;
	Poco::Net::HTTPResponse response;
	std::istream* pResponseStream;

	if (!_chunkedTransferEncoding)
	{
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, _endPointURI.getPathEtc(), Poco::Net::HTTPMessage::HTTP_1_1);
		prepareRequest(request, messageName);
		request.setContentLength(static_cast<int>(_pStream->tellp()));
		std::ostream& requestStream = _pSession->sendRequest(request);
		Poco::StreamCopier::copyStream(*_pStream, requestStream);
		pResponseStream = &_pSession->receiveResponse(response);

		if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED && (_authMode == AUTH_DIGEST || _authMode == AUTH_ANY))
		{
			Poco::NullOutputStream nullStream;
			Poco::StreamCopier::copyStream(*pResponseStream, nullStream);

			_creds.authenticate(request, response);
			std::ostream& requestStream = _pSession->sendRequest(request);
			_pStream->clear();
			_pStream->seekg(0);
			Poco::StreamCopier::copyStream(*_pStream, requestStream);
			pResponseStream = &_pSession->receiveResponse(response);
		}
	}
	else
	{
		pResponseStream = &_pSession->receiveResponse(response);
	}
	if (_pCookieStore)
	{
		_pCookieStore->updateCookies(response);
	}

	if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
	{
		if (response.get("Content-Encoding"s, ""s) == "gzip")
		{
			Poco::InflatingInputStream inflater(*pResponseStream, Poco::InflatingStreamBuf::STREAM_GZIP);
			_deserializer.setup(inflater);
		}
		else
		{
			_deserializer.setup(*pResponseStream);
		}

		// consume extra response content
		int ch = pResponseStream->get();
		while (ch != -1) ch = pResponseStream->get();

		return _deserializer;
	}
	else
	{
		Poco::NullOutputStream nullStream;
		Poco::StreamCopier::copyStream(*pResponseStream, nullStream);
		throw Poco::RemotingNG::TransportException("HTTP Error", response.getReason());
	}
}


void Transport::endRequest()
{
	_serializer.reset();
	_deserializer.reset();
	_pStream = 0;
	_pDeflater = 0;
}


const Poco::Net::HTTPClientSession& Transport::requireSession() const
{
	if (_pSession)
		return *_pSession;
	else
		throw Poco::IllegalStateException("Transport not connected");
}


Poco::Net::HTTPClientSession& Transport::requireSession()
{
	if (_pSession)
		return *_pSession;
	else
		throw Poco::IllegalStateException("Transport not connected");
}


void Transport::prepareRequest(Poco::Net::HTTPRequest& request, const std::string& /*messageName*/)
{
	request.setContentType(CONTENT_TYPE);
	request.setChunkedTransferEncoding(_chunkedTransferEncoding);
	request.set("Accept-Encoding"s, "gzip"s);
	if (_compression && _chunkedTransferEncoding)
	{
		request.set("Content-Encoding"s, "gzip"s);
	}
	if (_authMode == AUTH_BASIC)
	{
		Poco::Net::HTTPBasicCredentials creds(_creds.getUsername(), _creds.getPassword());
		creds.authenticate(request);
	}
	if (!_userAgent.empty())
	{
		request.set(USER_AGENT, _userAgent);
	}
	for (const auto& p: _customHeaders)
	{
		request.set(p.first, p.second);
	}
	if (_pCookieStore)
	{
		_pCookieStore->addCookies(request);
	}
}


} } } // namespace Poco::RemotingNG::JSONRPC
