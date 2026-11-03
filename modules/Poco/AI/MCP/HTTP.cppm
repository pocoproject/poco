//
// HTTP.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_AI_MCP_HTTP
#include "Poco/AI/MCP/HTTP/Body.h"
#include "Poco/AI/MCP/HTTP/Connection.h"
#include "Poco/AI/MCP/HTTP/EventStreamReader.h"
#include "Poco/AI/MCP/HTTP/HTTPClient.h"
#include "Poco/AI/MCP/HTTP/HTTP.h"
#include "Poco/AI/MCP/HTTP/ServerRequestHandler.h"
#include "Poco/AI/MCP/HTTP/ServerTransport.h"
#include "Poco/AI/MCP/HTTP/SessionFactory.h"
#endif

export module Poco:AI.MCP.HTTP;

export namespace Poco::AI::MCP::HTTP {
	#ifdef ENABLE_AI_MCP_HTTP
	using Poco::AI::MCP::HTTP::EventStreamReader;
	using Poco::AI::MCP::HTTP::HTTPClient;
	using Poco::AI::MCP::HTTP::ServerRequestHandler;
	using Poco::AI::MCP::HTTP::ServerRequestHandlerFactory;
	using Poco::AI::MCP::HTTP::ServerTransport;

	using Poco::AI::MCP::HTTP::ReadGuard;
	using Poco::AI::MCP::HTTP::SessionFactory;

	using Poco::AI::MCP::HTTP::createClientSession;
	#endif
}
