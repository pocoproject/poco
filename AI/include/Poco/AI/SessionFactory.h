//
// SessionFactory.h
//
// Library: AI
// Package: Core
// Module:  SessionFactory
//
// The session factory hook of the providers and of MCPHost. It is the one
// the MCP HTTP transport defines, so an application sets up HTTP, HTTPS,
// TLS contexts and proxies in one place for both.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_SessionFactory_INCLUDED
#define AI_SessionFactory_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/AI/MCP/HTTP/SessionFactory.h"


namespace Poco {
namespace AI {


using SessionFactory = Poco::AI::MCP::HTTP::SessionFactory;
	/// Creates the Poco::Net::HTTPClientSession for one request to a URI;
	/// see Poco::AI::MCP::HTTP::SessionFactory.


using Poco::AI::MCP::HTTP::createClientSession;
	/// The default session factory; see
	/// Poco::AI::MCP::HTTP::createClientSession(). An https endpoint needs
	/// Poco::Net::HTTPSSessionInstantiator registered by the application.


} } // namespace Poco::AI


#endif // AI_SessionFactory_INCLUDED
