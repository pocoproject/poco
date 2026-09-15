//
// Transport.cpp
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  Transport
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/SOAP/Transport.h"
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
#include "Poco/SHA1Engine.h"
#include "Poco/Base64Encoder.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Random.h"
#include <sstream>


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace SOAP {


const std::string Transport::PROTOCOL("soap");
const std::string Transport::PROTOCOL_SOAP11("soap-1.1");
const std::string Transport::PROTOCOL_SOAP12("soap-1.2");
const std::string Transport::SOAPACTION("SOAPAction");
const std::string Transport::USER_AGENT("User-Agent");
const std::string Transport::PROP_ACTION("action");
const std::string Transport::CONTENT_TYPE_SOAP11("text/xml; charset=\"utf-8\"");
const std::string Transport::CONTENT_TYPE_SOAP12("application/soap+xml; charset=\"utf-8\"");


Poco::Net::HTTPSessionFactory Transport::_httpSessionFactory;


class WSSEInterceptor: public Serializer::HeaderInterceptor
{
public:
	WSSEInterceptor(const Transport& transport):
		_transport(transport)
	{
	}

	void interceptHeader(XMLElement& headerElem)
	{
		if (_transport.getAuthentication() == Transport::AUTH_WSSE_TEXT || _transport.getAuthentication() == Transport::AUTH_WSSE_DIGEST)
		{
			const auto& credentials = _transport.getCredentials();

			const std::string username = credentials.getAttribute(Credentials::ATTR_USERNAME);
			const std::string password = credentials.getAttribute(Credentials::ATTR_PASSWORD);

			XMLElement securityElem("Security"s, Serializer::WSSENAMESPACE);
			XMLElement usernameTokenElem("UsernameToken"s, Serializer::WSSENAMESPACE);
			XMLElement usernameElem("Username"s, Serializer::WSSENAMESPACE, username);
			usernameTokenElem.addChild(std::move(usernameElem));

			if (_transport.getAuthentication() == Transport::AUTH_WSSE_TEXT)
			{
				XMLElement passwordElem("Password"s,  Serializer::WSSENAMESPACE, password);
				usernameTokenElem.addChild(std::move(passwordElem));
			}
			else 
			{
				const std::string created = Poco::DateTimeFormatter::format(Poco::DateTime(), Poco::DateTimeFormat::ISO8601_FORMAT);
				const std::string nonce = createNonce();
				const std::string nonce64 = base64(nonce); 

				Poco::SHA1Engine sha1;
				sha1.update(nonce);
				sha1.update(created);
				sha1.update(password);
				const auto digest = sha1.digest();
				const std::string digest64 = base64(digest);

				XMLElement passwordElem("Password"s,  Serializer::WSSENAMESPACE, digest64);
				passwordElem.setAttribute("Type"s, ""s, "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest"s);
				XMLElement nonceElem("Nonce"s, Serializer::WSSENAMESPACE, nonce64);
				XMLElement createdElem("Created"s, Serializer::WSUNAMESPACE, created);

				usernameTokenElem.addChild(std::move(passwordElem));
				usernameTokenElem.addChild(std::move(nonceElem));
				usernameTokenElem.addChild(std::move(createdElem));
			}
			securityElem.addChild(std::move(usernameTokenElem));
			headerElem.addChild(std::move(securityElem));
		}
	}

protected:
	template <typename T>
	std::string base64(const T& data)
	{
		std::stringstream encStream;
		Poco::Base64Encoder enc(encStream);
		enc.write(reinterpret_cast<const char*>(data.data()), data.size());
		enc.close();
		return encStream.str();
	}

	std::string createNonce() const
	{
		std::ostringstream nonce;
		Poco::Base64Encoder enc(nonce, Poco::BASE64_URL_ENCODING | Poco::BASE64_NO_PADDING);
		Poco::Random rnd;
		rnd.seed();
		for (int i = 0; i < 16; i++)
		{
			enc.put(rnd.nextChar());
		}
		enc.close();
		return nonce.str();
	}

private:
	const Transport& _transport;
};


Transport::Transport():
	_keepAlive(false),
	_chunkedTransferEncoding(true),
	_compression(false),
	_mtom(false),
	_authMode(AUTH_NONE),
	_pCookieStore(new CookieStore)
{
}


Transport::Transport(Serializer::SOAPVersion soapVersion):
	_serializer(soapVersion),
	_keepAlive(false),
	_chunkedTransferEncoding(true),
	_compression(false),
	_mtom(false),
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


bool Transport::isMTOMEnabled() const
{
	return _mtom;
}


void Transport::enableMTOM(bool enable)
{
	_mtom = enable;
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


void Transport::setSerializerOptions(int options)
{
	_serializer.setOptions(options);
}


void Transport::addRequestHeaderInterceptor(RequestHeaderInterceptor::Ptr pInterceptor)
{
	_serializer.addHeaderInterceptor(pInterceptor);
}


void Transport::removeRequestHeaderInterceptor(RequestHeaderInterceptor::Ptr pInterceptor)
{
	_serializer.removeHeaderInterceptor(pInterceptor);
}


void Transport::addResponseHeaderInterceptor(ResponseHeaderInterceptor::Ptr pInterceptor)
{
	_deserializer.addHeaderInterceptor(pInterceptor);
}


void Transport::removeResponseHeaderInterceptor(ResponseHeaderInterceptor::Ptr pInterceptor)
{
	_deserializer.removeHeaderInterceptor(pInterceptor);
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
		_serializer.enableMTOM(_mtom);
		if (_mtom)
		{
			_mtomContentType = _serializer.createMTOMContentType(getAttribute(PROP_ACTION, ""s));
		}

		if ((_authMode == AUTH_WSSE_TEXT || _authMode == AUTH_WSSE_DIGEST) && !_pWSSEInterceptor)
		{
			_pWSSEInterceptor = new WSSEInterceptor(*this);
			_serializer.addHeaderInterceptor(_pWSSEInterceptor);
		}
		else if (_authMode != AUTH_WSSE_TEXT && _authMode != AUTH_WSSE_DIGEST && _pWSSEInterceptor)
		{
			_serializer.removeHeaderInterceptor(_pWSSEInterceptor);
			_pWSSEInterceptor.reset();
		}

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

	if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK ||
	    (response.getStatus() == Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR &&
	    (response.getContentType().find("text/xml") == 0 || response.getContentType().find("application/soap+xml") == 0 || Poco::icompare(response.getContentType(), 0, 17, "Multipart/Related") == 0)))
	{
		if (Poco::icompare(response.getContentType(), 0, 17, "Multipart/Related") == 0)
		{
			_deserializer.enableMTOM(true);
			_deserializer.parseContentType(response.getContentType());
		}
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
	if (_mtom)
	{
		request.setContentType(_mtomContentType);
	}
	else if (_serializer.getSOAPVersion() == Serializer::SOAP_1_1)
	{
		request.setContentType(CONTENT_TYPE_SOAP11);
	}
	else
	{
		std::string contentType(CONTENT_TYPE_SOAP12);
		std::string soapAction(getAttribute(PROP_ACTION, ""s));
		if (!soapAction.empty())
		{
			contentType.append("; action=\"");
			contentType.append(soapAction);
			contentType.append("\"");
		}
		request.setContentType(contentType);
	}
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
	if (_serializer.getSOAPVersion() == Serializer::SOAP_1_1)
	{
		std::string soapAction("\"");
		soapAction.append(getAttribute(PROP_ACTION, ""s));
		soapAction.append("\"");
		request.set(SOAPACTION, soapAction);
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


} } } // namespace Poco::RemotingNG::SOAP
