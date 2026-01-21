//
// RESTRequestHandler.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  RESTRequestHandler
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/RESTRequestHandler.h"
#include "Poco/RemotingNG/REST/Serializer.h"
#include "Poco/RemotingNG/REST/Deserializer.h"
#include "Poco/RemotingNG/REST/Transport.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerTransport.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/StringTokenizer.h"
#include "Poco/StreamCopier.h"
#include "Poco/InflatingStream.h"
#include "Poco/DeflatingStream.h"
#include <sstream>


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace REST {


class RESTServerTransport: public Poco::RemotingNG::ServerTransport
{
public:
	RESTServerTransport(Listener& listener, const Credentials& credentials, Poco::Logger& logger):
		_listener(listener),
		_credentials(credentials),
		_logger(logger)
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

	void reportException(const std::string& method, const Poco::Exception& exc)
	{
		if (_listener.areServiceExceptionsLogged())
		{
			_logger.error("Exception in %s: %s"s, method, exc.displayText());
		}
	}

private:
	const Listener& _listener;
	const Credentials& _credentials;
	Poco::Logger& _logger;
};



class ChunkedServerTransport: public RESTServerTransport
{
public:
	ChunkedServerTransport(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, Listener& listener, const Poco::RemotingNG::Credentials& credentials, Poco::Logger& logger):
		RESTServerTransport(listener, credentials, logger),
		_request(request),
		_response(response),
		_enableCompression(listener.isCompressionEnabled()),
		_pInflater(0),
		_pDeflater(0),
		_replySent(false)
	{
	}

	~ChunkedServerTransport()
	{
		delete _pInflater;
		delete _pDeflater;
	}

	Poco::RemotingNG::Deserializer& beginRequest()
	{
		if (_request.get("Content-Encoding"s, ""s) == "gzip")
		{
			_pInflater = new Poco::InflatingInputStream(_request.stream(), Poco::InflatingStreamBuf::STREAM_GZIP);
			_deserializer.setup(*_pInflater);
		}
		else
		{
			_deserializer.setup(_request.stream());
		}
		_deserializer.setRequest(_request);
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

		_response.setChunkedTransferEncoding(true);
		_response.setContentType(getAttribute(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, Transport::CONTENT_TYPE));
		bool compressResponse(_enableCompression && _request.hasToken("Accept-Encoding"s, "gzip"s) && messageType != Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
		if (compressResponse)
		{
			_response.set("Content-Encoding"s, "gzip"s);
			_pDeflater = new Poco::DeflatingOutputStream(_response.send(), Poco::DeflatingStreamBuf::STREAM_GZIP, 1);
			_serializer.setup(*_pDeflater);
			_replySent = true;
		}
		else
		{
			if (messageType == Poco::RemotingNG::SerializerBase::MESSAGE_FAULT)
			{
				_serializer.setup(_faultStream);
			}
			else
			{
				_serializer.setup(_response.send());
				_replySent = true;
			}
		}
		_serializer.setResponse(_response);
		return _serializer;
	}

	void endRequest()
	{
		if (_pDeflater) _pDeflater->close();
		if (!_replySent) // one-way message
		{
			_replySent = true;
			std::ostream& responseStream = _response.send();
			Poco::StreamCopier::copyStream(_faultStream, responseStream);
		}
	}

private:
	Poco::Net::HTTPServerRequest& _request;
	Poco::Net::HTTPServerResponse& _response;
	Poco::RemotingNG::REST::Serializer _serializer;
	Poco::RemotingNG::REST::Deserializer _deserializer;
	bool _enableCompression;
	Poco::InflatingInputStream* _pInflater;
	Poco::DeflatingOutputStream* _pDeflater;
	std::stringstream _faultStream;
	bool _replySent;
};


class NonChunkedServerTransport: public RESTServerTransport
{
public:
	NonChunkedServerTransport(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, Listener& listener, const Poco::RemotingNG::Credentials& credentials, Poco::Logger& logger):
		RESTServerTransport(listener, credentials, logger),
		_request(request),
		_response(response),
		_enableCompression(listener.isCompressionEnabled()),
		_pInflater(0),
		_pDeflater(0)
	{
	}

	~NonChunkedServerTransport()
	{
		delete _pInflater;
		delete _pDeflater;
	}

	Poco::RemotingNG::Deserializer& beginRequest()
	{
		if (_request.get("Content-Encoding"s, ""s) == "gzip")
		{
			_pInflater = new Poco::InflatingInputStream(_request.stream(), Poco::InflatingStreamBuf::STREAM_GZIP);
			_deserializer.setup(*_pInflater);
		}
		else
		{
			_deserializer.setup(_request.stream());
		}
		_deserializer.setRequest(_request);
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

		_response.setContentType(getAttribute(Poco::RemotingNG::SerializerBase::PROP_CONTENT_TYPE, Transport::CONTENT_TYPE));
		bool compressResponse(_enableCompression && _request.hasToken("Accept-Encoding"s, "gzip"s) && messageType != Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
		if (compressResponse)
		{
			_response.set("Content-Encoding"s, "gzip"s);
			_pDeflater = new Poco::DeflatingOutputStream(_bufferStream, Poco::DeflatingStreamBuf::STREAM_GZIP, 1);
			_serializer.setup(*_pDeflater);
		}
		else
		{
			_serializer.setup(_bufferStream);
		}
		_serializer.setResponse(_response);
		return _serializer;
	}

	void endRequest()
	{
		if (_pDeflater) _pDeflater->close();
		_response.setContentLength(static_cast<int>(_bufferStream.tellp()));
		std::ostream& ostr = _response.send();
		Poco::StreamCopier::copyStream(_bufferStream, ostr);
	}

private:
	Poco::Net::HTTPServerRequest& _request;
	Poco::Net::HTTPServerResponse& _response;
	Poco::RemotingNG::REST::Serializer _serializer;
	Poco::RemotingNG::REST::Deserializer _deserializer;
	bool _enableCompression;
	Poco::InflatingInputStream* _pInflater;
	Poco::DeflatingOutputStream* _pDeflater;
	std::stringstream _bufferStream;
};


RESTRequestHandler::RESTRequestHandler(Listener& listener):
	_listener(listener),
	_logger(Poco::Logger::get("RemotingNG.REST.RESTRequestHandler"s))
{
}


RESTRequestHandler::~RESTRequestHandler()
{
}


void RESTRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
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
	}

	const std::string& method = request.getMethod();
	if (_listener.isCORSEnabled() && method == Poco::Net::HTTPRequest::HTTP_OPTIONS && request.has("Origin"s))
	{
		if (handleCORS(request, response))
		{
			response.set("Access-Control-Allow-Methods"s, "HEAD, GET, PUT, POST, PATCH, DELETE"s);
			response.set("Access-Control-Allow-Headers"s, _listener.getCORSAllowedHeaders());
		}
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

	if (_listener.isCORSEnabled() && request.has("Origin"s))
	{
		if (!handleCORS(request, response))
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
			response.setContentLength(0);
			response.send();
			return;
		}
	}

	Poco::RemotingNG::Credentials credentials;
	Poco::RemotingNG::ScopedContext scopedContext;
	scopedContext.context()->setValue("transport"s, _listener.protocol());
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
		else if (scheme == "Bearer")
		{
			credentials.setAttribute(Credentials::ATTR_MECHANISM, scheme);
			credentials.setAttribute(Credentials::ATTR_TOKEN, authInfo);
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
		ChunkedServerTransport transport(request, response, _listener, credentials, _logger);
		bool serviceFound = orb.invoke(_listener, uri, transport);
		if (!serviceFound)
		{
			Poco::NotFoundException exc(request.getURI());
			transport.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT).serializeFaultMessage(""s, exc);
			transport.endRequest();
		}
	}
	else
	{
		NonChunkedServerTransport transport(request, response, _listener, credentials, _logger);
		bool serviceFound = orb.invoke(_listener, uri, transport);
		if (!serviceFound)
		{
			Poco::NotFoundException exc(request.getURI());
			transport.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT).serializeFaultMessage(""s, exc);
			transport.endRequest();
		}
	}
}


bool RESTRequestHandler::handleCORS(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	const std::string& allowedOrigin = _listener.getCORSAllowedOrigin();
	if (allowedOrigin.empty())
	{
		response.set("Access-Control-Allow-Origin"s, request.get("Origin"s));
		response.set("Access-Control-Allow-Credentials"s, "true"s);
		return true;
	}
	else if (allowedOrigin == "*" || allowedOrigin == request.get("Origin"s))
	{
		response.set("Access-Control-Allow-Origin"s, allowedOrigin);
		response.set("Access-Control-Allow-Credentials"s, "true"s);
		return true;
	}
	return false;
}


} } } // namespace Poco::RemotingNG::REST
