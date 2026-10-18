//
// ToolRegistry.h
//
// Library: AI
// Package: Agent
// Module:  ToolRegistry
//
// A thread-safe registry of the tools an agent may call.
//
// Copyright (c) 2025-2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_ToolRegistry_INCLUDED
#define AI_ToolRegistry_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/AI/MCP/Tool.h"
#include "Poco/AI/MCP/Content.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/JSON/Object.h"
#include "Poco/Mutex.h"
#include <functional>
#include <map>
#include <string>
#include <vector>


namespace Poco::AI {


[[nodiscard]] AI_API std::string toolResultText(const Poco::AI::MCP::ToolResult& result);
	/// The text of a tool result for a model: its text blocks joined with
	/// newlines; without text blocks, its structured content as JSON, or else
	/// its content blocks as JSON.


class AI_API ToolRegistry
	/// A registry of the tools an agent may call, described as MCP tools: a
	/// tool has a name, a description, an input schema and optional output
	/// schema and annotations, and its handler returns an MCP tool result
	/// with an error flag. MCP tools of a remote server and native tools are
	/// thus one model.
	///
	/// Thread-safe: tools may be registered and run concurrently.
{
public:
	using TextExecutor = std::function<std::string (const Poco::JSON::Object::Ptr& input)>;
		/// A tool that returns text. Its text becomes the text block of the
		/// result; an exception becomes an error result.

	ToolRegistry();
		/// Creates an empty registry.

	~ToolRegistry();
		/// Destroys the registry.

	void registerTool(const Poco::AI::MCP::Tool& tool);
		/// Registers a tool, replacing one of the same name. The schema and
		/// annotation objects must not be changed after registration.
		///
		/// Throws Poco::InvalidArgumentException if the name is empty or the
		/// tool has no handler.

	[[nodiscard]] bool registerToolIfAbsent(const Poco::AI::MCP::Tool& tool);
		/// Registers a tool unless one of the same name is registered, as one
		/// step. Returns true if it registered the tool.
		///
		/// Throws Poco::InvalidArgumentException if the name is empty or the
		/// tool has no handler.

	void registerTool(const std::string& name, const std::string& description, const Poco::JSON::Object::Ptr& inputSchema, TextExecutor executor);
		/// Registers a tool that returns text, replacing one of the same name.
		///
		/// Throws Poco::InvalidArgumentException if the name is empty or the
		/// executor is empty.

	void unregisterTool(const std::string& name);
		/// Unregisters a tool.

	[[nodiscard]] std::vector<Poco::AI::MCP::Tool> tools() const;
		/// Returns the tools in name order. The schema objects are shared with
		/// the registry: serialize copies (Tool::toObject() makes them).

	[[nodiscard]] Poco::AI::MCP::ToolResult executeTool(const std::string& name, const Poco::JSON::Object::Ptr& input, const Poco::AI::MCP::RequestContext& context = Poco::AI::MCP::RequestContext());
		/// Runs a tool with the given input (null for none) and request context.
		/// An unknown tool and a handler that throws give an error result, never
		/// an exception.

	[[nodiscard]] bool hasTool(const std::string& name) const;
		/// Returns true if a tool of that name is registered.

	[[nodiscard]] std::size_t size() const;
		/// Returns the number of tools.

private:
	ToolRegistry(const ToolRegistry&) = delete;
	ToolRegistry& operator = (const ToolRegistry&) = delete;

	mutable Poco::FastMutex _mutex;
	std::map<std::string, Poco::AI::MCP::Tool> _tools;
};


} // namespace Poco::AI


#endif // AI_ToolRegistry_INCLUDED
