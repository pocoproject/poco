//
// RequestContext.h
//
// Library: AI/MCP
// Package: Core
// Module:  RequestContext
//
// Definition of the Identity and RequestContext structs.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_RequestContext_INCLUDED
#define AIMCP_RequestContext_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Implementation.h"
#include "Poco/JSON/Object.h"
#include <functional>
#include <string>


namespace Poco::AI::MCP {


struct AIMCP_API Identity
	/// The caller of a request, as the transport authenticated it. The
	/// library never fills it in itself: an authenticating transport sets
	/// it, so that handlers can make authorization decisions.
{
	std::string principal;
		/// The authenticated user or client, e.g. the subject of a token.
	std::string scope;
		/// Free-form authorization scope, e.g. the scopes of a token.
	bool authenticated = false;
		/// Whether the transport authenticated the caller.
};


struct AIMCP_API RequestContext
	/// What a handler knows about the request it serves. The transport sets
	/// the identity; the server adds what the request's _meta object carries,
	/// since every request of the stateless protocol describes its client
	/// anew.
{
	Identity identity;
		/// Set by the transport.

	std::string protocolVersion;
		/// The protocol revision the request names.

	Poco::JSON::Object::Ptr clientCapabilities;
		/// The capabilities the client declared with the request; never
		/// null within a handler.

	Implementation clientInfo;
		/// The client's name and version, if the request carried them.

	std::function<bool ()> cancelled = [] { return false; };
		/// Returns true once the work for the request is to be abandoned; a
		/// handler that runs long asks it now and then, best through
		/// isCancelled(). It returns false unless a transport or an agent
		/// loop sets it: the Server serves a request synchronously, and an
		/// agent loop asks the Cancellation of its run besides the function it
		/// was given. It is valid while the handler runs, not after.

	[[nodiscard]] bool isCancelled() const;
		/// Returns what cancelled returns; false when it is empty.
};


//
// inlines
//
inline bool RequestContext::isCancelled() const
{
	return cancelled && cancelled();
}


} // namespace Poco::AI::MCP


#endif // AIMCP_RequestContext_INCLUDED
