//
// Server.h
//
// Library: AIMCP
// Package: Core
// Module:  Server
//
// The MCP server facade: register tools, then accept messages either at the
// message level (handleMessage, used by the HTTP transport) or as a stream pump
// (serve, used by the stdio transport). Transport-agnostic - depends only on
// Poco::JSON, never Poco::Net.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Server_INCLUDED
#define AIMCP_Server_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Dispatcher.h"
#include "Poco/AI/MCP/Tool.h"
#include "Poco/AI/MCP/Resource.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/JSON/Object.h"
#include <iosfwd>
#include <string>


namespace Poco {
namespace AI {
namespace MCP {


class AIMCP_API Server
	/// An MCP server. Register tools, set server info, then expose it through any
	/// transport via the two ingress entry points.
{
public:
	Server();
		/// Creates an empty server.

	~Server();
		/// Destroys the server.

	void registerTool(const std::string& name, const std::string& description,
		Poco::JSON::Object::Ptr inputSchema, ToolHandler handler);
		/// Registers a tool exposed via tools/list and tools/call.

	void registerTool(const Tool& tool);
		/// Registers a fully-specified tool (e.g. with an outputSchema, annotations,
		/// or a context-aware handler).

	void registerResource(const Resource& resource, ResourceReader reader);
		/// Registers a resource exposed via resources/list and read via resources/read.

	void registerPrompt(const Prompt& prompt, PromptRenderer renderer);
		/// Registers a prompt exposed via prompts/list and rendered via prompts/get.

	void setServerInfo(const std::string& name, const std::string& version);
		/// Sets the name/version reported during initialize.

	void setInstructions(const std::string& instructions);
		/// Sets the optional instructions returned by initialize.

	Poco::JSON::Object::Ptr handleMessage(const Poco::JSON::Object::Ptr& message);
		/// Message-level ingress: handles one already-parsed JSON-RPC message and
		/// returns the response envelope, or null for a notification.

	Poco::JSON::Object::Ptr handleMessage(const Poco::JSON::Object::Ptr& message, const RequestContext& context);
		/// Message-level ingress that threads a per-request context (identity) into
		/// the tool handler. Used by authenticating transports.

	void serve(std::istream& in, std::ostream& out);
		/// Stream-level ingress: reads newline-delimited JSON-RPC messages from in,
		/// writes the replies to out, and returns at end of stream. Malformed lines
		/// and unsupported batch arrays produce JSON-RPC error replies.

	Dispatcher& dispatcher();
		/// Returns the underlying dispatcher (for advanced/transport use).

private:
	Server(const Server&) = delete;
	Server& operator = (const Server&) = delete;

	Dispatcher _dispatcher;
};


//
// inlines
//
inline Dispatcher& Server::dispatcher()
{
	return _dispatcher;
}


} } } // namespace Poco::AI::MCP


#endif // AIMCP_Server_INCLUDED
