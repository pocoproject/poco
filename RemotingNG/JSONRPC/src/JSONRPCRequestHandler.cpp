//
// JSONRPCRequestHandler.cpp
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  JSONRPCRequestHandler
//
// Copyright (c) 2006-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/JSONRPC/JSONRPCRequestHandler.h"
#include "Poco/RemotingNG/JSONRPC/Serializer.h"
#include "Poco/RemotingNG/JSONRPC/Deserializer.h"
#include "Poco/RemotingNG/JSONRPC/Transport.h"
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
namespace JSONRPC {


class JSONRPCServerTransport: public Poco::RemotingNG::ServerTransport
{
public:
	JSONRPCServerTransport(Listener& listener, const Credentials& credentials):
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

private:
	const Listener& _listener;
	const Credentials& _credentials;
};


class ChunkedServerTransport: public JSONRPCServerTransport
{
public:
	ChunkedServerTransport(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, Listener& listener, const Poco::RemotingNG::Credentials& credentials):
		JSONRPCServerTransport(listener, credentials),
		_request(request),
		_response(response),
		_enableCompression(listener.isCompressionEnabled()),
		_pDeflater(0),
		_replySent(false)
	{
		_deserializer.associateSerializer(_serializer);
	}

	~ChunkedServerTransport()
	{
		delete _pDeflater;
	}

	Poco::RemotingNG::Deserializer& beginRequest()
	{
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

		_response.setChunkedTransferEncoding(true);
		_response.setContentType(Transport::CONTENT_TYPE);
		bool compressResponse(_enableCompression && _request.hasToken("Accept-Encoding"s, "gzip"s));
		if (compressResponse)
		{
			_response.set("Content-Encoding"s, "gzip");
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
	Poco::RemotingNG::JSONRPC::Serializer _serializer;
	Poco::RemotingNG::JSONRPC::Deserializer _deserializer;
	bool _enableCompression;
	Poco::DeflatingOutputStream* _pDeflater;
	bool _replySent;
};


class NonChunkedServerTransport: public JSONRPCServerTransport
{
public:
	NonChunkedServerTransport(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response, Listener& listener, const Poco::RemotingNG::Credentials& credentials):
		JSONRPCServerTransport(listener, credentials),
		_request(request),
		_response(response)
	{
		_deserializer.associateSerializer(_serializer);
	}

	Poco::RemotingNG::Deserializer& beginRequest()
	{
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

		_response.setContentType(Transport::CONTENT_TYPE);
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
	Poco::RemotingNG::JSONRPC::Serializer _serializer;
	Poco::RemotingNG::JSONRPC::Deserializer _deserializer;
	std::stringstream _bufferStream;
};


JSONRPCRequestHandler::JSONRPCRequestHandler(Listener& listener):
	_listener(listener),
	_logger(Poco::Logger::get("RemotingNG.JSONRPC.JSONRPCRequestHandler"s))
{
}


JSONRPCRequestHandler::~JSONRPCRequestHandler()
{
}


void JSONRPCRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
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
	if (method == Poco::Net::HTTPRequest::HTTP_HEAD)
	{
		response.send();
		return;
	}
	else if (_listener.isCORSEnabled() && method == Poco::Net::HTTPRequest::HTTP_OPTIONS && request.has("Origin"))
	{
		if (handleCORS(request, response))
		{
			response.set("Access-Control-Allow-Methods"s, "POST"s);
			response.set("Access-Control-Allow-Headers"s, "Accept, Content-Type"s);
		}
		response.setContentLength(0);
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


bool JSONRPCRequestHandler::handleCORS(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
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


} } } // namespace Poco::RemotingNG::JSONRPC
