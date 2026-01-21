//
// SOAPRequestHandler.cpp
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  SOAPRequestHandler
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/SOAP/SOAPRequestHandler.h"
#include "Poco/RemotingNG/SOAP/Serializer.h"
#include "Poco/RemotingNG/SOAP/Deserializer.h"
#include "Poco/RemotingNG/SOAP/Transport.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerTransport.h"
#include "Poco/RemotingNG/Credentials.h"
#include "Poco/StringTokenizer.h"
#include "Poco/StreamCopier.h"
#include "Poco/InflatingStream.h"
#include "Poco/DeflatingStream.h"
#include <sstream>


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace SOAP {


const std::string SOAPRequestHandler::SOAPACTION("SOAPAction");
const std::string SOAPRequestHandler::EMPTY;


class SOAPServerTransport;


class WSSEAuthRequestHeaderInterceptor: public Listener::RequestHeaderInterceptor
{
public:
	WSSEAuthRequestHeaderInterceptor(SOAPServerTransport& transport):
		_transport(transport)
	{
	}

	void interceptHeader(const XMLElement& header);

private:
	SOAPServerTransport& _transport;
};


class SOAPServerTransport: public Poco::RemotingNG::ServerTransport
{
public:
	SOAPServerTransport(Listener& listener, Credentials& credentials):
		_listener(listener),
		_credentials(credentials)
	{
	}

	bool authenticate(const std::string& /*method*/)
	{
		Authenticator::Ptr pAuth = _listener.getAuthenticator();
		if (pAuth)
		{
			return pAuth->authenticate(_credentials).done();
		}
		return false;
	}

	bool authorize(const std::string& method, const std::string& permission)
	{
		Authorizer::Ptr pAuth = _listener.getAuthorizer();
		if (pAuth)
		{
			return pAuth->authorize(method, permission);
		}
		return false;
	}

	const Listener& listener() const
	{
		return _listener;
	}

	Credentials& credentials() 
	{
		return _credentials;
	}

protected:
	void applyRequestHeaderInterceptors(Deserializer& deserializer)
	{
		if (_listener.isWSSEEnabled())
		{
			deserializer.addHeaderInterceptor(new WSSEAuthRequestHeaderInterceptor(*this));
		}
		for (auto pInterceptor: _listener.requestHeaderInterceptors())
		{
			deserializer.addHeaderInterceptor(pInterceptor);
		}
	}

private:
	const Listener& _listener;
	Credentials& _credentials;
};


void WSSEAuthRequestHeaderInterceptor::interceptHeader(const XMLElement& header)
{
	const Poco::RemotingNG::SOAP::XMLElement* pSecurityElem = header.findChild("Security"s, Serializer::WSSENAMESPACE);
	if (pSecurityElem)
	{
		const Poco::RemotingNG::SOAP::XMLElement* pUsernameTokenElem = pSecurityElem->findChild("UsernameToken", Serializer::WSSENAMESPACE);
		if (pUsernameTokenElem)
		{	
			Credentials& credentials = _transport.credentials();
	
			const Poco::RemotingNG::SOAP::XMLElement* pUsernameElem = pUsernameTokenElem->findChild("Username"s, Serializer::WSSENAMESPACE);
			if (pUsernameElem) credentials.setAttribute(Credentials::ATTR_USERNAME, pUsernameElem->value());

			const Poco::RemotingNG::SOAP::XMLElement* pPasswordElem = pUsernameTokenElem->findChild("Password"s, Serializer::WSSENAMESPACE);
			if (pPasswordElem) 
			{
				credentials.setAttribute(Credentials::ATTR_PASSWORD, pPasswordElem->value());
				const std::string& type = pPasswordElem->attributes().getValue(""s, "Type"s);
				if (!type.empty())
				{
					credentials.setAttribute(Credentials::ATTR_MECHANISM, type);
				}
			}

			const Poco::RemotingNG::SOAP::XMLElement* pNonceElem = pUsernameTokenElem->findChild("Nonce"s, Serializer::WSSENAMESPACE);
			if (pNonceElem) credentials.setAttribute(Credentials::ATTR_NONCE, pNonceElem->value());

			const Poco::RemotingNG::SOAP::XMLElement* pCreatedElem = pUsernameTokenElem->findChild("Created"s, Serializer::WSUNAMESPACE);
			if (pCreatedElem) credentials.setAttribute(Credentials::ATTR_CREATED, pCreatedElem->value());

			Poco::RemotingNG::ScopedContext scopedContext;
			scopedContext.context()->setCredentials(credentials);
		}
	}
}


class ChunkedServerTransport: public SOAPServerTransport
{
public:
	ChunkedServerTransport(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, Listener& listener, Poco::RemotingNG::Credentials& credentials):
		SOAPServerTransport(listener, credentials),
		_request(request),
		_response(response),
		_enableCompression(listener.isCompressionEnabled()),
		_enableMTOM(listener.isMTOMEnabled()),
		_pDeflater(0),
		_replySent(false)
	{
		applyRequestHeaderInterceptors(_deserializer);
	}

	~ChunkedServerTransport()
	{
		delete _pDeflater;
	}

	Poco::RemotingNG::Deserializer& beginRequest()
	{
		if (Poco::icompare(_request.getContentType(), 0, 17, "Multipart/Related") == 0)
		{
			_deserializer.enableMTOM(true);
			_deserializer.parseContentType(_request.getContentType());
		}
		if (_request.get("Content-Encoding"s, "") == "gzip")
		{
			Poco::InflatingInputStream inflater(_request.stream(), Poco::InflatingStreamBuf::STREAM_GZIP);
			_deserializer.setup(inflater);
		}
		else
		{
			_deserializer.setup(_request.stream());
		}
		return _deserializer;
	}

	Poco::RemotingNG::Serializer& sendReply(Poco::RemotingNG::SerializerBase::MessageType messageType)
	{
		// consume extra request content
		std::istream& istr = _request.stream();
		int ch = istr.get();
		while (ch != -1) ch = istr.get();

		_response.setChunkedTransferEncoding(true);
		if (messageType == Poco::RemotingNG::SerializerBase::MESSAGE_FAULT)
		{
			_response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
		}
		if (_enableMTOM && messageType != Poco::RemotingNG::SerializerBase::MESSAGE_FAULT)
		{
			_response.setContentType(_serializer.createMTOMContentType());
			_serializer.enableMTOM(true);
		}
		else
		{
			_response.setContentType(_deserializer.soapVersion() == Serializer::SOAP_1_1 ? Transport::CONTENT_TYPE_SOAP11 : Transport::CONTENT_TYPE_SOAP12);
			_serializer.enableMTOM(false);
		}
		_serializer.setSOAPVersion(_deserializer.soapVersion());

		bool compressResponse(_enableCompression && _request.hasToken("Accept-Encoding"s, "gzip"s));
		if (compressResponse)
		{
			_response.set("Content-Encoding"s, "gzip"s);
			_pDeflater = new Poco::DeflatingOutputStream(_response.send(), Poco::DeflatingStreamBuf::STREAM_GZIP, 1);
			_serializer.setup(*_pDeflater);
		}
		else
		{
			_serializer.setup(_response.send());
		}
		_replySent = true;
		return _serializer;
	}

	void endRequest()
	{
		if (!_replySent) // one-way message
		{
			_replySent = true;
			_response.send();
		}
	}

private:
	Poco::Net::HTTPServerRequest& _request;
	Poco::Net::HTTPServerResponse& _response;
	Poco::RemotingNG::SOAP::Serializer _serializer;
	Poco::RemotingNG::SOAP::Deserializer _deserializer;
	bool _enableCompression;
	bool _enableMTOM;
	Poco::DeflatingOutputStream* _pDeflater;
	bool _replySent;
};


class NonChunkedServerTransport: public SOAPServerTransport
{
public:
	NonChunkedServerTransport(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, Listener& listener, Poco::RemotingNG::Credentials& credentials):
		SOAPServerTransport(listener, credentials),
		_request(request),
		_response(response),
		_enableMTOM(listener.isMTOMEnabled())
	{
		applyRequestHeaderInterceptors(_deserializer);
	}

	Poco::RemotingNG::Deserializer& beginRequest()
	{
		if (Poco::icompare(_request.getContentType(), 0, 17, "Multipart/Related") == 0)
		{
			_deserializer.enableMTOM(true);
			_deserializer.parseContentType(_request.getContentType());
		}
		if (_request.get("Content-Encoding"s, ""s) == "gzip")
		{
			Poco::InflatingInputStream inflater(_request.stream(), Poco::InflatingStreamBuf::STREAM_GZIP);
			_deserializer.setup(inflater);
		}
		else
		{
			_deserializer.setup(_request.stream());
		}
		return _deserializer;
	}

	Poco::RemotingNG::Serializer& sendReply(Poco::RemotingNG::SerializerBase::MessageType messageType)
	{
		// consume extra request content
		std::istream& istr = _request.stream();
		int ch = istr.get();
		while (ch != -1) ch = istr.get();

		if (messageType == Poco::RemotingNG::SerializerBase::MESSAGE_FAULT)
		{
			_response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
		}
		if (_enableMTOM && messageType != Poco::RemotingNG::SerializerBase::MESSAGE_FAULT)
		{
			_response.setContentType(_serializer.createMTOMContentType());
			_serializer.enableMTOM(true);
		}
		else
		{
			_response.setContentType(_deserializer.soapVersion() == Serializer::SOAP_1_1 ? Transport::CONTENT_TYPE_SOAP11 : Transport::CONTENT_TYPE_SOAP12);
			_serializer.enableMTOM(false);
		}
		_serializer.setSOAPVersion(_deserializer.soapVersion());
		_serializer.setup(_bufferStream);
		return _serializer;
	}

	void endRequest()
	{
		_response.setContentLength(static_cast<int>(_bufferStream.tellp()));
		std::ostream& ostr = _response.send();
		Poco::StreamCopier::copyStream(_bufferStream, ostr);
	}

private:
	Poco::Net::HTTPServerRequest& _request;
	Poco::Net::HTTPServerResponse& _response;
	bool _enableMTOM;
	Poco::RemotingNG::SOAP::Serializer _serializer;
	Poco::RemotingNG::SOAP::Deserializer _deserializer;
	std::stringstream _bufferStream;
};


SOAPRequestHandler::SOAPRequestHandler(Listener& listener):
	_listener(listener),
	_logger(Poco::Logger::get("RemotingNG.SOAP.SOAPRequestHandler"s))
{
}


SOAPRequestHandler::~SOAPRequestHandler()
{
}


void SOAPRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	if (_logger.debug())
	{
		std::string message(request.getMethod());
		message += " ";
		message += request.getURI();
		message += " ";
		message += request.getVersion();
		message += " from ";
		message += request.clientAddress().toString();
		_logger.debug(message);
		if (request.has(SOAPACTION))
		{
			_logger.debug("SOAP Action: " + request[SOAPACTION]);
		}
	}

	const std::string& method = request.getMethod();
	if (method == Poco::Net::HTTPRequest::HTTP_HEAD)
	{
		response.send();
		return;
	}
	else if (method != Poco::Net::HTTPRequest::HTTP_POST)
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
		response.setContentLength(0);
		response.send();
		return;
	}
	std::string ce = request.get("Content-Encoding"s, ""s);
	if (!ce.empty() && ce != "gzip")
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_UNSUPPORTED_MEDIA_TYPE);
		response.setContentLength(0);
		response.send();
		return;
	}

	Poco::RemotingNG::Credentials credentials;
	Poco::RemotingNG::ScopedContext scopedContext;
	scopedContext.context()->setValue("transport"s, Transport::PROTOCOL);
	scopedContext.context()->setValue("remoteAddress"s, request.clientAddress());
	scopedContext.context()->setValue("localAddress"s, request.serverAddress());
	scopedContext.context()->setValue("uri"s, request.getURI());
	scopedContext.context()->clearCredentials();
	if (request.hasCredentials())
	{
		std::string scheme;
		std::string authInfo;
		request.getCredentials(scheme, authInfo);
		if (scheme == Poco::Net::HTTPBasicCredentials::SCHEME)
		{
			Poco::Net::HTTPBasicCredentials creds(authInfo);
			scopedContext.context()->setValue("username"s, creds.getUsername());
			scopedContext.context()->setValue("password"s, creds.getPassword());
			credentials.setAttribute(Credentials::ATTR_USERNAME, creds.getUsername());
			credentials.setAttribute(Credentials::ATTR_PASSWORD, creds.getPassword());
			scopedContext.context()->setCredentials(credentials);
		}
	}
	scopedContext.context()->setValue("httpRequest"s, &request);
	scopedContext.context()->setValue("httpResponse"s, &response);

	const std::string& uri = request.getURI();
	Poco::RemotingNG::ORB& orb = Poco::RemotingNG::ORB::instance();
	bool supportsChunked = (_listener.isChunkedTransferEncodingEnabled() && request.getVersion() != Poco::Net::HTTPServerRequest::HTTP_1_0);
	if (supportsChunked)
	{
		ChunkedServerTransport transport(request, response, _listener, credentials);
		bool serviceFound = orb.invoke(_listener, uri, transport);
		if (!serviceFound)
		{
			RemotingException exc("Unknown service");
			transport.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT).serializeFaultMessage("", exc);
			transport.endRequest();
		}
	}
	else
	{
		NonChunkedServerTransport transport(request, response, _listener, credentials);
		bool serviceFound = orb.invoke(_listener, uri, transport);
		if (!serviceFound)
		{
			RemotingException exc("Unknown service");
			transport.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT).serializeFaultMessage("", exc);
			transport.endRequest();
		}
	}
}


} } } // namespace Poco::RemotingNG::SOAP
