//
// Dispatcher.h
//
// Library: AIMCP
// Package: Core
// Module:  Dispatcher
//
// The transport-agnostic protocol core: a JSON-RPC method router plus the tool
// registry. handle() takes one parsed message and returns the response object,
// or null when no reply is due (a notification).
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Dispatcher_INCLUDED
#define AIMCP_Dispatcher_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/AI/MCP/Tool.h"
#include "Poco/AI/MCP/Resource.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/JSON/Object.h"
#include "Poco/Mutex.h"
#include <map>
#include <string>


namespace Poco {
namespace AI {
namespace MCP {


class AIMCP_API Dispatcher
	/// Routes JSON-RPC messages to the MCP method handlers and holds the tool
	/// registry. Thread-safe: tools are registered before serving and read under
	/// a lock during dispatch.
{
public:
	Dispatcher();
		/// Creates a Dispatcher with empty server info and no tools.

	~Dispatcher();
		/// Destroys the Dispatcher.

	void registerTool(const Tool& tool);
		/// Adds (or replaces) a tool by name.

	void registerResource(const Resource& resource, ResourceReader reader);
		/// Adds (or replaces) a resource by uri, exposed via resources/list and read
		/// via resources/read. Enables the "resources" capability.

	void registerPrompt(const Prompt& prompt, PromptRenderer renderer);
		/// Adds (or replaces) a prompt by name, exposed via prompts/list and rendered
		/// via prompts/get. Enables the "prompts" capability.

	void setServerInfo(const std::string& name, const std::string& version);
		/// Sets the name/version reported in the initialize result's serverInfo.

	void setInstructions(const std::string& instructions);
		/// Sets the optional human-readable instructions returned by initialize.

	Poco::JSON::Object::Ptr handle(const Poco::JSON::Object::Ptr& message);
		/// Handles one JSON-RPC message with an empty request context and returns the
		/// response envelope, or null for a notification.

	Poco::JSON::Object::Ptr handle(const Poco::JSON::Object::Ptr& message, const RequestContext& context);
		/// Handles one JSON-RPC message, threading the request context into a
		/// tools/call handler. Returns the response envelope, or null for a
		/// notification (which never produces a reply).

	bool initialized() const;
		/// Returns whether an initialize request has been handled.

private:
	Poco::JSON::Object::Ptr onInitialize(const Id& id, const Poco::JSON::Object::Ptr& params);
	Poco::JSON::Object::Ptr onToolsList(const Id& id, const Poco::JSON::Object::Ptr& params);
	Poco::JSON::Object::Ptr onToolsCall(const Id& id, const Poco::JSON::Object::Ptr& params, const RequestContext& context);
	Poco::JSON::Object::Ptr onResourcesList(const Id& id, const Poco::JSON::Object::Ptr& params);
	Poco::JSON::Object::Ptr onResourcesRead(const Id& id, const Poco::JSON::Object::Ptr& params);
	Poco::JSON::Object::Ptr onPromptsList(const Id& id, const Poco::JSON::Object::Ptr& params);
	Poco::JSON::Object::Ptr onPromptsGet(const Id& id, const Poco::JSON::Object::Ptr& params);
	void onNotification(const std::string& method, const Poco::JSON::Object::Ptr& params);

	Dispatcher(const Dispatcher&) = delete;
	Dispatcher& operator = (const Dispatcher&) = delete;

	struct ResourceEntry { Resource resource; ResourceReader reader; };
	struct PromptEntry { Prompt prompt; PromptRenderer renderer; };

	mutable Poco::FastMutex _mutex;
	std::map<std::string, Tool> _tools;
	std::map<std::string, ResourceEntry> _resources;
	std::map<std::string, PromptEntry> _prompts;
	std::string _serverName;
	std::string _serverVersion;
	std::string _instructions;
	bool _initialized;
};


} } } // namespace Poco::AI::MCP


#endif // AIMCP_Dispatcher_INCLUDED
