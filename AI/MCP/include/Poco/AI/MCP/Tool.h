//
// Tool.h
//
// Library: AIMCP
// Package: Core
// Module:  Tool
//
// An MCP tool descriptor (name, description, JSON Schema input) and the handler
// callback invoked for tools/call.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Tool_INCLUDED
#define AIMCP_Tool_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Content.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/JSON/Object.h"
#include <functional>
#include <string>


namespace Poco {
namespace AI {
namespace MCP {


using ToolHandler = std::function<ToolResult (const Poco::JSON::Object::Ptr& arguments)>;
	/// Invoked for tools/call. Receives the (never-null) arguments object and
	/// returns the content to send back. Throwing is allowed - the dispatcher
	/// turns an exception into a ToolResult with isError set.


using ContextToolHandler = std::function<ToolResult (const Poco::JSON::Object::Ptr& arguments, const RequestContext& context)>;
	/// Like ToolHandler, but also receives the per-request context (identity) so a
	/// handler can make authorization decisions. A tool sets exactly one of the two
	/// handler forms; when both are set the context form wins.


struct AIMCP_API Tool
	/// Describes a tool the server exposes via tools/list and tools/call.
{
	std::string name;
	std::string description;
	Poco::JSON::Object::Ptr inputSchema;
		/// A JSON Schema object describing the tool's arguments.
	Poco::JSON::Object::Ptr outputSchema;
		/// Optional JSON Schema for the tool's structured result. Advertised in
		/// tools/list when set.
	Poco::JSON::Object::Ptr annotations;
		/// Optional tool annotations (e.g. readOnlyHint, destructiveHint,
		/// idempotentHint). Advertised in tools/list when set.
	ToolHandler handler;
	ContextToolHandler contextHandler;
};


} } } // namespace Poco::AI::MCP


#endif // AIMCP_Tool_INCLUDED
