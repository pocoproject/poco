//
// Content.h
//
// Library: AI/MCP
// Package: Core
// Module:  Content
//
// Definition of the ToolResult class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Content_INCLUDED
#define AIMCP_Content_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Resource.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/Dynamic/Var.h"
#include <string>


namespace Poco::AI::MCP {


class AIMCP_API ToolResult
	/// The result of a tool call: content blocks, optional structured content
	/// and an error flag. Serializes to the tools/call result
	/// {"content": [...], "structuredContent"?: value, "isError": bool}.
	///
	/// A tool that returns structured content should also return it serialized
	/// in a text block, for clients that read only the content blocks. A tool
	/// that declares an outputSchema must return structured content that
	/// conforms to it.
{
public:
	ToolResult();
		/// Creates an empty result that is not an error.

	[[nodiscard]] static ToolResult error(const std::string& text);
		/// Returns an error result with one text block.

	ToolResult& addText(const std::string& text);
		/// Appends a text block. Returns *this.

	ToolResult& addImage(const std::string& data, const std::string& mimeType);
		/// Appends an image block; data is the image in base64. Returns *this.

	ToolResult& addAudio(const std::string& data, const std::string& mimeType);
		/// Appends an audio block; data is the audio in base64. Returns *this.

	ToolResult& addResourceLink(const std::string& uri, const std::string& name, const std::string& mimeType = std::string());
		/// Appends a link to a resource the client can read. Returns *this.

	ToolResult& addResource(const ResourceContents& contents);
		/// Appends a resource embedded with its contents. Returns *this.

	ToolResult& addContent(const Poco::JSON::Object::Ptr& block);
		/// Appends a content block as it is, e.g. one taken from another
		/// result. Returns *this.
		///
		/// Throws Poco::InvalidArgumentException if block is null or has no
		/// string type.

	ToolResult& setError(bool isError = true);
		/// Marks the result as an error, or not. Returns *this.

	ToolResult& setStructuredContent(const Poco::Dynamic::Var& structured);
		/// Sets the structured content: any JSON value, e.g. a
		/// Poco::JSON::Object::Ptr. An empty Var removes it. Returns *this.

	[[nodiscard]] bool isError() const;
		/// Returns whether the result is a tool execution error.

	[[nodiscard]] Poco::JSON::Array::Ptr content() const;
		/// Returns the array of content blocks.

	[[nodiscard]] const Poco::Dynamic::Var& structuredContent() const;
		/// Returns the structured content, or an empty Var if there is none.

	[[nodiscard]] Poco::JSON::Object::Ptr toObject() const;
		/// Serializes the result to the tools/call result object.

	[[nodiscard]] static ToolResult fromObject(const Poco::JSON::Object::Ptr& object);
		/// Parses a tools/call result object.
		///
		/// Throws Poco::DataFormatException if content is missing or not an
		/// array of objects with a string type, or if isError is present and
		/// not a boolean.

private:
	Poco::JSON::Array::Ptr _pContent;
	Poco::Dynamic::Var _structuredContent;
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
	return _pContent;
}


inline const Poco::Dynamic::Var& ToolResult::structuredContent() const
{
	return _structuredContent;
}


} // namespace Poco::AI::MCP


#endif // AIMCP_Content_INCLUDED
