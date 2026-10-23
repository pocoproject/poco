//
// ServerRequestHandler.h
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  ServerRequestHandler
//
// Definition of the ServerRequestHandler and ServerRequestHandlerFactory
// classes.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCPHTTP_ServerRequestHandler_INCLUDED
#define AIMCPHTTP_ServerRequestHandler_INCLUDED


#include "Poco/AI/MCP/HTTP/HTTP.h"
#include "Poco/AI/MCP/HTTP/ServerTransport.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include <string>


namespace Poco::AI::MCP::HTTP {


class AIMCPHTTP_API ServerRequestHandler: public Poco::Net::HTTPRequestHandler
	/// Serves one request to an MCP endpoint with a ServerTransport, for a
	/// Poco::Net::HTTPServer whose factory creates a handler per request.
	///
	/// The caller is not signed in: the request context has no identity. A
	/// host that authenticates its callers serves them with its own handler,
	/// which calls ServerTransport::handle() with the context of the caller.
{
public:
	ServerRequestHandler(Server& server, const ServerTransport& transport);
		/// Creates the handler. The server and the transport must outlive it.

	~ServerRequestHandler() override;
		/// Destroys the handler.

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
		/// Answers the request with the transport.

private:
	Server& _server;
	const ServerTransport& _transport;
};


class AIMCPHTTP_API ServerRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	/// The factory of an HTTPServer that serves one MCP endpoint and nothing
	/// else: a ServerRequestHandler for the requests to the path of the
	/// endpoint, and 404 for any other path.
	///
	/// The HTTPServer must leave the request headers as they were sent, with
	/// Poco::Net::HTTPServerParams::setAutoDecodeHeaders(false); see
	/// ServerTransport.
{
public:
	ServerRequestHandlerFactory(Server& server, const std::string& path, const ServerTransport::Params& params = ServerTransport::Params());
		/// Creates the factory of the endpoint at path, e.g. "/mcp". The
		/// server must outlive the HTTPServer, which owns the factory.

	~ServerRequestHandlerFactory() override;
		/// Destroys the factory.

	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;
		/// Returns the handler of the request.

	[[nodiscard]] const ServerTransport& transport() const;
		/// Returns the transport of the endpoint.

private:
	Server& _server;
	std::string _path;
	ServerTransport _transport;
};


//
// inlines
//
inline const ServerTransport& ServerRequestHandlerFactory::transport() const
{
	return _transport;
}


} // namespace Poco::AI::MCP::HTTP


#endif // AIMCPHTTP_ServerRequestHandler_INCLUDED
