//
// Server.h
//
// Library: AI/MCP
// Package: Core
// Module:  Server
//
// Definition of the Server class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Server_INCLUDED
#define AIMCP_Server_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/AI/MCP/Tool.h"
#include "Poco/AI/MCP/Resource.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/AI/MCP/Implementation.h"
#include "Poco/JSON/Object.h"
#include "Poco/Mutex.h"
#include "Poco/Timespan.h"
#include <iosfwd>
#include <map>
#include <optional>
#include <string>


namespace Poco::AI::MCP {


class AIMCP_API Server
	/// An MCP server of protocol revision 2026-07-28. Register tools,
	/// resources and prompts, then serve requests through any transport:
	/// message by message with handle(), or as a newline-delimited stream
	/// with serve() (the stdio transport serves std::cin and std::cout).
	///
	/// The protocol is stateless. Every request carries the protocol revision
	/// and the client's capabilities in its _meta object, so one Server serves
	/// any number of clients and connections at once. It answers:
	///
	///   - server/discover, with the supported revision, the capabilities and
	///     the instructions;
	///   - tools/list and tools/call;
	///   - resources/list, resources/templates/list (always empty) and
	///     resources/read, once a resource is registered;
	///   - prompts/list and prompts/get, once a prompt is registered.
	///
	/// server/discover advertises the resources and prompts capabilities only
	/// then, and their methods get METHOD_NOT_FOUND before, as the
	/// specification has it for a capability the server does not advertise.
	///
	/// Every result carries resultType "complete" and, when set, the server
	/// info in its _meta object. Lists come in one page, in name order.
	/// Notifications get no reply; notifications/cancelled is accepted, and
	/// has no effect because requests are served synchronously.
	///
	/// Registration is thread-safe. Handlers run on the thread that calls
	/// handle() or serve(), outside the server's lock.
	///
	/// A stdio server must keep std::cout for the protocol: nothing else may
	/// write to it, so the application sends its log output to std::cerr or
	/// to a file.
{
public:
	enum class CacheScope
		/// Whether caches may share what the server lists and returns.
	{
		PUBLIC,  /// The same for every caller.
		PRIVATE  /// May depend on the caller's authorization.
	};

	Server();
		/// Creates a server without tools, resources and prompts.

	~Server();
		/// Destroys the server.

	void registerTool(const Tool& tool);
		/// Registers a tool, replacing a tool of the same name. The schema and
		/// annotation objects must not be changed after registration. A tool
		/// without an input schema is listed with {"type": "object"}.
		///
		/// Throws Poco::InvalidArgumentException if the name is empty, the
		/// tool has no handler, or its input schema does not have the type
		/// "object".

	void registerTool(const std::string& name, const std::string& description, const Poco::JSON::Object::Ptr& inputSchema, ToolHandler handler);
		/// Registers a tool whose handler does not need the request context.
		///
		/// Throws Poco::InvalidArgumentException if the name is empty, the
		/// handler is empty, or the input schema does not have the type
		/// "object".

	[[nodiscard]] std::optional<Tool> findTool(const std::string& name) const;
		/// Returns the definition of the tool of that name, or nothing. A
		/// transport checks with it what a call carries besides its message,
		/// e.g. the headers the input schema asks of a call over HTTP.

	void registerResource(const Resource& resource, ResourceReader reader);
		/// Registers a resource, replacing one with the same URI.
		///
		/// Throws Poco::InvalidArgumentException if the URI is empty or the
		/// reader is empty.

	void registerPrompt(const Prompt& prompt, PromptRenderer renderer);
		/// Registers a prompt, replacing one of the same name.
		///
		/// Throws Poco::InvalidArgumentException if the name is empty or the
		/// renderer is empty.

	void setServerInfo(const Implementation& info);
		/// Sets the name and version the server reports in the _meta object
		/// of every result, Implementation::library() by default. Without a
		/// name, results carry no server info; the specification asks a
		/// server to send it unless it is told not to.

	void setInstructions(const std::string& instructions);
		/// Sets the instructions server/discover returns.

	void setCacheHints(const Poco::Timespan& ttl, CacheScope scope);
		/// Sets the caching hints of the results of server/discover, the lists
		/// and resources/read: how long a client may keep them, and whether
		/// caches may share them. The default is zero, stale at once, and
		/// PRIVATE.

	void setLoggerName(const std::string& name);
		/// Sets the logger to which the server writes, at the error level,
		/// why a request failed, "AI.MCP.Server" by default. The reply to
		/// such a request says no more than "Internal error".

	[[nodiscard]] Poco::JSON::Object::Ptr handle(const Poco::JSON::Object::Ptr& message, const RequestContext& context = RequestContext());
		/// Message-level ingress: handles one parsed JSON-RPC message and
		/// returns the reply, or null for a notification. The transport sets
		/// the identity of the context; the server adds what the request's
		/// _meta object carries. Never throws: an exception of a handler or
		/// of the server becomes an error reply.
		///
		/// A message that is neither a valid request nor a valid notification
		/// (no jsonrpc 2.0, no method, an id that is neither a string nor an
		/// integer) gets an INVALID_REQUEST reply, without an id when it has
		/// none that can be read. Only a valid notification gets no reply.

	void serve(std::istream& in, std::ostream& out, std::size_t maxMessageSize = DEFAULT_MAX_MESSAGE_SIZE);
		/// Stream-level ingress: reads newline-delimited messages from in,
		/// writes the replies to out, and returns at the end of in. A line
		/// that is not JSON gets a Parse error reply; a line that is JSON but
		/// not an object (a scalar, or a batch) or is longer than
		/// maxMessageSize bytes gets an Invalid Request reply. These replies
		/// carry no id.
		///
		/// Throws Poco::WriteFileException when out fails, so that a server
		/// whose client has gone away stops.

private:
	struct ResourceEntry
	{
		Resource resource;
		ResourceReader reader;
	};

	struct PromptEntry
	{
		Prompt prompt;
		PromptRenderer renderer;
	};

	struct Settings
	{
		Implementation serverInfo;
		std::string instructions;
		Poco::Timespan ttl;
		CacheScope scope = CacheScope::PRIVATE;
		std::string loggerName = "AI.MCP.Server";
	};

	Poco::JSON::Object::Ptr dispatch(const Id& id, const std::string& method, const Poco::JSON::Object::Ptr& params, const RequestContext& context);
	Poco::JSON::Object::Ptr failed(const Id& id, const std::string& method, const std::string& what) const;
	Poco::JSON::Object::Ptr onDiscover(const Id& id);
	Poco::JSON::Object::Ptr onToolsList(const Id& id, const Poco::JSON::Object::Ptr& params);
	Poco::JSON::Object::Ptr onToolsCall(const Id& id, const Poco::JSON::Object::Ptr& params, const RequestContext& context);
	Poco::JSON::Object::Ptr onResourcesList(const Id& id, const Poco::JSON::Object::Ptr& params);
	Poco::JSON::Object::Ptr onResourceTemplatesList(const Id& id, const Poco::JSON::Object::Ptr& params);
	Poco::JSON::Object::Ptr onResourcesRead(const Id& id, const Poco::JSON::Object::Ptr& params, const RequestContext& context);
	Poco::JSON::Object::Ptr onPromptsList(const Id& id, const Poco::JSON::Object::Ptr& params);
	Poco::JSON::Object::Ptr onPromptsGet(const Id& id, const Poco::JSON::Object::Ptr& params, const RequestContext& context);
	Poco::JSON::Object::Ptr complete(const Id& id, Poco::JSON::Object::Ptr result, bool cacheable);
	Settings settings() const;

	Server(const Server&) = delete;
	Server& operator = (const Server&) = delete;

	mutable Poco::FastMutex _mutex;
	std::map<std::string, Tool> _tools;
	std::map<std::string, ResourceEntry> _resources;
	std::map<std::string, PromptEntry> _prompts;
	Settings _settings;
};


} // namespace Poco::AI::MCP


#endif // AIMCP_Server_INCLUDED
