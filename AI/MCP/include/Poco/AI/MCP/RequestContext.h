//
// RequestContext.h
//
// Library: AIMCP
// Package: Core
// Module:  RequestContext
//
// Per-request context carried from a transport into a tool handler. The core
// stays transport- and application-agnostic: a transport authenticates a request
// however it likes, fills in an Identity, and the Dispatcher threads the context
// down to the tool handler for that single call. Nothing here is stored on the
// shared Server/Dispatcher, so concurrent requests with different identities are
// independent.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_RequestContext_INCLUDED
#define AIMCP_RequestContext_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include <string>


namespace Poco {
namespace AI {
namespace MCP {


struct AIMCP_API Identity
	/// The authenticated principal behind a request, as resolved by the transport.
	/// Generic on purpose - no token or application types leak in.
{
	std::string principal;
		/// The authenticated user/subject, or empty when unauthenticated.

	std::string scope;
		/// An optional scope/role string the transport associated with the principal
		/// (e.g. a token scope). May be empty.

	bool authenticated = false;
		/// True when the transport positively authenticated the request.
};


struct AIMCP_API RequestContext
	/// Everything a tool handler may need to know about the request it is serving.
	/// Passed by const reference for a single tools/call and never retained.
{
	Identity identity;
};


} } } // namespace Poco::AI::MCP


#endif // AIMCP_RequestContext_INCLUDED
