//
// Content.h
//
// Library: AIMCP
// Package: Core
// Module:  Content
//
// The result of a tool invocation: a list of MCP content blocks plus an error
// flag. Per the spec, tool execution failures are reported here (isError = true)
// rather than as JSON-RPC errors, so the model sees the failure text.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Content_INCLUDED
#define AIMCP_Content_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include <string>


namespace Poco {
namespace AI {
namespace MCP {


class AIMCP_API ToolResult
	/// The content blocks and error flag returned by a tool handler. Serializes
	/// to {"content": [ ... ], "isError": bool}.
{
public:
	ToolResult();
		/// Creates an empty, non-error result.

	ToolResult& addText(const std::string& text);
		/// Appends a {"type": "text", "text": text} content block. Returns *this.

	ToolResult& setError(bool isError = true);
		/// Marks the result as an error (or not). Returns *this.

	ToolResult& setStructuredContent(const Poco::JSON::Object::Ptr& structured);
		/// Attaches a structured result object (serialized under "structuredContent").
		/// Per the spec a tool that declares an outputSchema SHOULD also return the
		/// serialized JSON in a text block for backwards compatibility, so callers
		/// typically pair this with addText(). Returns *this.

	bool isError() const;
		/// Returns whether this result represents a tool execution error.

	Poco::JSON::Array::Ptr content() const;
		/// Returns the array of content blocks.

	Poco::JSON::Object::Ptr toObject() const;
		/// Serializes to the MCP tools/call result object.

	static ToolResult fromObject(const Poco::JSON::Object::Ptr& object);
		/// Parses a tools/call result object back into a ToolResult.

private:
	Poco::JSON::Array::Ptr _content;
	Poco::JSON::Object::Ptr _structuredContent;
	bool _isError;
};


//
// inlines
//
inline bool ToolResult::isError() const
{
	return _isError;
}


inline Poco::JSON::Array::Ptr ToolResult::content() const
{
	return _content;
}


} } } // namespace Poco::AI::MCP


#endif // AIMCP_Content_INCLUDED
