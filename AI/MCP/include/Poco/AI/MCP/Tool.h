//
// Tool.h
//
// Library: AI/MCP
// Package: Core
// Module:  Tool
//
// Definition of the Tool struct and the tool handler types.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Tool_INCLUDED
#define AIMCP_Tool_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/ToolResult.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/JSON/Object.h"
#include <functional>
#include <string>
#include <vector>


namespace Poco::AI::MCP {


using ContextToolHandler = std::function<ToolResult (const Poco::JSON::Object::Ptr& arguments, const RequestContext& context)>;
	/// Invoked for tools/call with the never-null arguments object and the
	/// context of the request, so that a handler can make authorization
	/// decisions. Throwing is allowed: the server turns the exception into a
	/// result with isError set and the text of the exception, which the
	/// client, and a model that reads the result, gets. A handler whose
	/// exceptions may tell what a client must not know catches them itself.


using ToolHandler = std::function<ToolResult (const Poco::JSON::Object::Ptr& arguments)>;
	/// A handler that does not need the request context. Server::registerTool()
	/// takes it in its short form and wraps it into a ContextToolHandler.


struct AIMCP_API Tool
	/// Describes a tool the server lists in tools/list and runs for tools/call.
{
	std::string name;
		/// Should be 1 to 128 characters from A-Z, a-z, 0-9, '_', '-' and '.'.
	std::string title;
		/// Optional name for display.
	std::string description;
	Poco::JSON::Object::Ptr inputSchema;
		/// A JSON Schema object describing the arguments, with type object at
		/// its root. A tool without one is listed with {"type": "object"}.
	Poco::JSON::Object::Ptr outputSchema;
		/// Optional JSON Schema of the tool's structured content.
	Poco::JSON::Object::Ptr annotations;
		/// Optional hints, e.g. readOnlyHint or destructiveHint. Hints from a
		/// remote server are untrusted.
	ContextToolHandler handler;
		/// Required when the tool is registered with a server.

	struct HeaderAnnotation
		/// A parameter the input schema marks with x-mcp-header: the header
		/// name without the Mcp-Param- prefix, the property path from the
		/// root of the arguments, and the type of the parameter: string,
		/// integer or boolean.
	{
		std::string name;
		std::vector<std::string> path;
		std::string type;
	};

	[[nodiscard]] std::vector<HeaderAnnotation> headerAnnotations() const;
		/// Returns the x-mcp-header annotations of the input schema; none
		/// without a schema.
		///
		/// Throws Poco::DataFormatException for an annotation that is not
		/// valid: at the root of the schema, not a header name (a token), on
		/// a parameter that is not a string, an integer or a boolean, a name
		/// used twice (compared without case), or on a parameter reached
		/// through other keywords than properties, e.g. items, oneOf or $ref.

	[[nodiscard]] Poco::JSON::Object::Ptr toObject() const;
		/// Returns the tools/list entry. The schema and annotation objects
		/// are deep copies, so that replies serialized on different threads
		/// share no object.

	[[nodiscard]] static Tool fromObject(const Poco::JSON::Object::Ptr& object);
		/// Reads a tools/list entry; the handler stays empty.
		///
		/// Throws Poco::DataFormatException if the name is not a non-empty
		/// string or inputSchema is not an object.
};


} // namespace Poco::AI::MCP


#endif // AIMCP_Tool_INCLUDED
