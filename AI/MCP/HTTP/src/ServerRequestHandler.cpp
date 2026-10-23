//
// ServerRequestHandler.cpp
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  ServerRequestHandler
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/HTTP/ServerRequestHandler.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"


namespace Poco::AI::MCP::HTTP {


namespace
{
	class NotFoundHandler: public Poco::Net::HTTPRequestHandler
		/// Answers a request to any other path with 404.
	{
	public:
		void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse& response) override
		{
			response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
			response.setContentLength(0);
			response.send();
		}
	};
}


ServerRequestHandler::ServerRequestHandler(Server& server, const ServerTransport& transport):
	_server(server),
	_transport(transport)
{
}


ServerRequestHandler::~ServerRequestHandler()
{
}


void ServerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	_transport.handle(_server, request, response);
}


ServerRequestHandlerFactory::ServerRequestHandlerFactory(Server& server, const std::string& path, const ServerTransport::Params& params):
	_server(server),
	_path(path),
	_transport(params)
{
}


ServerRequestHandlerFactory::~ServerRequestHandlerFactory()
{
}


Poco::Net::HTTPRequestHandler* ServerRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
	std::string path;
	try
	{
		path = Poco::URI(request.getURI()).getPath();
	}
	catch (const Poco::SyntaxException&)
	{
		return new NotFoundHandler;
	}
	if (path != _path) return new NotFoundHandler;
	return new ServerRequestHandler(_server, _transport);
}


} // namespace Poco::AI::MCP::HTTP
