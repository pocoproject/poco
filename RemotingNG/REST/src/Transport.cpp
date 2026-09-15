//
// Transport.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  Transport
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/Transport.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSessionFactory.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/OAuth20Credentials.h"
#include "Poco/NullStream.h"
#include "Poco/InflatingStream.h"
#include "Poco/DeflatingStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace REST {


const std::string Transport::PROTOCOL("rest");
const std::string Transport::USER_AGENT("User-Agent");
const std::string Transport::CONTENT_TYPE("application/json; charset=\"utf-8\"");
const std::string Transport::BEARER("Bearer");
const std::string Transport::EMPTY;


Poco::Net::HTTPSessionFactory Transport::_httpSessionFactory;


Transport::Transport():
	_keepAlive(false),
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


void Transport::setUsername(const std::string& username)
{
	_creds.setAttribute(Credentials::ATTR_USERNAME, username);
}


const std::string& Transport::getUsername() const
{
	return _creds.getAttribute(Credentials::ATTR_USERNAME, EMPTY);
}


void Transport::setPassword( const std::string& password)
{
	_creds.setAttribute(Credentials::ATTR_PASSWORD, password);
}


const std::string& Transport::getPassword() const
{
	return _creds.getAttribute(Credentials::ATTR_PASSWORD, EMPTY);
}


void Transport::setCredentials(const Poco::RemotingNG::Credentials& creds)
{
	_creds = creds;
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
	_pRequest->setContentLength(static_cast<int>(_pStream->tellp()));
	std::ostream& requestStream = _pSession->sendRequest(*_pRequest);
	if (_pDeflater) _pDeflater->close();
	Poco::StreamCopier::copyStream(*_pStream, requestStream);

	_pResponse = new Poco::Net::HTTPResponse;
	std::istream& istr = _pSession->receiveResponse(*_pResponse);
	if (_pCookieStore)
	{
		_pCookieStore->updateCookies(*_pResponse);
	}
	Poco::NullOutputStream ostr;
	Poco::StreamCopier::copyStream(istr, ostr);
}


Poco::RemotingNG::Serializer& Transport::beginRequest(const Poco::RemotingNG::Identifiable::ObjectId& /*oid*/, const Poco::RemotingNG::Identifiable::TypeId& /*tid*/, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType)
{
	if (messageType == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST)
	{
		_pRequest = new Poco::Net::HTTPRequest(Poco::Net::HTTPRequest::HTTP_POST, _endPointURI.getPathEtc(), Poco::Net::HTTPMessage::HTTP_1_1);
		prepareRequest(messageName);

		_pStream = new std::stringstream;
		if (_compression)
		{
			_pDeflater = new Poco::DeflatingOutputStream(*_pStream, Poco::DeflatingStreamBuf::STREAM_GZIP, 1);
			_serializer.setup(*_pDeflater);
		}
		else
		{
			_serializer.setup(*_pStream);
		}
		_serializer.setRequest(*_pRequest);

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
	std::istream* pResponseStream;

	_pResponse = new Poco::Net::HTTPResponse;
	_pRequest->setContentLength(static_cast<int>(_pStream->tellp()));
	std::ostream& requestStream = _pSession->sendRequest(*_pRequest);
	if (_pDeflater) _pDeflater->close();
	Poco::StreamCopier::copyStream(*_pStream, requestStream);
	pResponseStream = &_pSession->receiveResponse(*_pResponse);

	if (_pResponse->getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED && (_authMode == AUTH_DIGEST || _authMode == AUTH_ANY))
	{
		Poco::NullOutputStream nullStream;
		Poco::StreamCopier::copyStream(*pResponseStream, nullStream);

		_httpCreds.authenticate(*_pRequest, *_pResponse);
		std::ostream& requestStream = _pSession->sendRequest(*_pRequest);
		_pStream->clear();
		_pStream->seekg(0);
		Poco::StreamCopier::copyStream(*_pStream, requestStream);
		pResponseStream = &_pSession->receiveResponse(*_pResponse);
	}
	if (_pCookieStore)
	{
		_pCookieStore->updateCookies(*_pResponse);
	}
	if (_pResponse->getStatus() < 400)
	{
		if (_pResponse->get("Content-Encoding"s, ""s) == "gzip")
		{
			_pInflater = new Poco::InflatingInputStream(*pResponseStream, Poco::InflatingStreamBuf::STREAM_GZIP);
			_deserializer.setup(*_pInflater);
		}
		else
		{
			_deserializer.setup(*pResponseStream);
		}
		_deserializer.setResponse(*_pResponse);

		return _deserializer;
	}
	else
	{
		std::string jsonError;
		if (_pResponse->get("Content-Encoding"s, ""s) == "gzip")
		{
			Poco::InflatingInputStream inflater(*pResponseStream, Poco::InflatingStreamBuf::STREAM_GZIP);
			Poco::StreamCopier::copyToString(inflater, jsonError);
		}
		else
		{
			Poco::StreamCopier::copyToString(*pResponseStream, jsonError);
		}
		try
		{
			if (jsonError.empty() || _pResponse->getContentType().compare(0, 16, "application/json"s) != 0)
			{
				throw Poco::RemotingNG::TransportException("HTTP Error", _pResponse->getReason(), static_cast<int>(_pResponse->getStatus()));
			}
			else
			{
				std::istringstream istr(jsonError);
				JSONDeserializer deser(istr);
				deser.deserializeError(_pResponse->getReason(), _pResponse->getStatus());
			}
		}
		catch (Poco::RemotingNG::TransportException&)
		{
			throw;
		}
		catch (Poco::RemotingNG::RemoteException& exc)
		{
			if (exc.message().empty())
				throw Poco::RemotingNG::RemoteException(jsonError, _pResponse->getStatus());
			else
				throw;
		}
		catch (Poco::Exception&)
		{
			throw Poco::RemotingNG::RemoteException(_pResponse->getReason(), jsonError, _pResponse->getStatus());
		}

		return _deserializer; // should never be reached
	}
}


void Transport::endRequest()
{
	_serializer.reset();
	_deserializer.reset();
	_pStream = 0;
	_pInflater = 0;
	_pDeflater = 0;
	_pRequest = 0;
	_pResponse = 0;
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


void Transport::prepareRequest(const std::string& messageName)
{
	std::string method;
	if (messageName == "delete_"s)
	{
		method = "DELETE";
	}
	else
	{
		method = Poco::toUpper(messageName);
	}
	_pRequest->setMethod(method);
	if (method == "POST" || method == "PUT" || method == "PATCH")
	{
		_pRequest->setContentType(getAttribute("contentType"s, CONTENT_TYPE));
	}
	_pRequest->setChunkedTransferEncoding(false);
	_pRequest->set("Accept-Encoding"s, "gzip"s);
	if (_compression)
	{
		_pRequest->set("Content-Encoding"s, "gzip"s);
	}
	if (_authMode == AUTH_BASIC || _authMode == AUTH_DIGEST || _authMode == AUTH_ANY)
	{
		_httpCreds.setUsername(_creds.getAttribute(Credentials::ATTR_USERNAME, EMPTY));
		_httpCreds.setPassword(_creds.getAttribute(Credentials::ATTR_PASSWORD, EMPTY));
	}
	if (_authMode == AUTH_BASIC)
	{
		Poco::Net::HTTPBasicCredentials creds(_httpCreds.getUsername(), _httpCreds.getPassword());
		creds.authenticate(*_pRequest);
	}
	else if (_authMode == AUTH_BEARER)
	{
		Poco::Net::OAuth20Credentials creds(_creds.getAttribute(Credentials::ATTR_TOKEN), _creds.getAttribute(Credentials::ATTR_MECHANISM, BEARER));
		creds.authenticate(*_pRequest);
	}
	else if (_authMode == AUTH_CUSTOM)
	{
		_pRequest->set(_creds.getAttribute(Credentials::ATTR_MECHANISM), _creds.getAttribute(Credentials::ATTR_TOKEN));
	}
	if (!_userAgent.empty())
	{
		_pRequest->set(USER_AGENT, _userAgent);
	}
	for (const auto& p: _customHeaders)
	{
		_pRequest->set(p.first, p.second);
	}
	if (_pCookieStore)
	{
		_pCookieStore->addCookies(*_pRequest);
	}
}


} } } // namespace Poco::RemotingNG::REST
