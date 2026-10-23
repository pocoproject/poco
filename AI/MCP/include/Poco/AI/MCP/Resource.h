//
// Resource.h
//
// Library: AI/MCP
// Package: Core
// Module:  Resource
//
// Definition of the Resource, ResourceContents, PromptArgument and Prompt
// structs and their handler types.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Resource_INCLUDED
#define AIMCP_Resource_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include <functional>
#include <string>
#include <vector>


namespace Poco::AI::MCP {


struct AIMCP_API Resource
	/// Describes a resource listed by resources/list and read by resources/read.
{
	std::string uri;
	std::string name;
	std::string title;
		/// Optional name for display.
	std::string description;
	std::string mimeType;

	[[nodiscard]] Poco::JSON::Object::Ptr toObject() const;
		/// Returns the resources/list entry; the optional members only when set.

	[[nodiscard]] static Resource fromObject(const Poco::JSON::Object::Ptr& object);
		/// Reads a resources/list entry.
		///
		/// Throws Poco::DataFormatException if uri or name is not a string.
};


struct AIMCP_API ResourceContents
	/// The contents of a resource, as resources/read returns them: text, or
	/// binary data encoded in base64.
{
	std::string uri;
	std::string mimeType;
	std::string text;
		/// The contents of a textual resource.
	std::string blob;
		/// The contents of a binary resource, in base64. When set, the
		/// contents are binary and text is not sent.
	bool binary = false;
		/// Whether the contents are binary also when blob is empty: a
		/// resource of zero bytes is sent with an empty blob, not as text.
		/// fromObject() sets it for contents that carry a blob.

	[[nodiscard]] Poco::JSON::Object::Ptr toObject() const;
		/// Returns the JSON form: {uri, mimeType?, blob} when the contents are
		/// binary (binary set, or blob not empty), {uri, mimeType?, text}
		/// otherwise.

	[[nodiscard]] static ResourceContents fromObject(const Poco::JSON::Object::Ptr& object);
		/// Reads the JSON form.
		///
		/// Throws Poco::DataFormatException if uri is not a string, or if the
		/// object carries neither a text nor a blob string.
};


using ResourceReader = std::function<ResourceContents (const std::string& uri, const RequestContext& context)>;
	/// Returns the contents of the resource identified by uri. Throwing is
	/// allowed: an MCPException becomes an error reply with its code and
	/// message; any other exception becomes an Internal error reply without
	/// its text, which the server logs (see Server::setLoggerName()).


struct AIMCP_API PromptArgument
	/// An argument a prompt declares (listed by prompts/list).
{
	std::string name;
	std::string title;
		/// Optional name for display.
	std::string description;
	bool required = false;
		/// A prompts/get request without a required argument is refused.
};


struct AIMCP_API Prompt
	/// Describes a prompt listed by prompts/list and rendered by prompts/get.
{
	std::string name;
	std::string title;
		/// Optional name for display.
	std::string description;
	std::vector<PromptArgument> arguments;

	[[nodiscard]] Poco::JSON::Object::Ptr toObject() const;
		/// Returns the prompts/list entry.

	[[nodiscard]] static Prompt fromObject(const Poco::JSON::Object::Ptr& object);
		/// Reads a prompts/list entry.
		///
		/// Throws Poco::DataFormatException if name is not a string.
};


using PromptRenderer = std::function<Poco::JSON::Array::Ptr (const Poco::JSON::Object::Ptr& arguments, const RequestContext& context)>;
	/// Renders a prompt into the messages array prompts/get returns. Receives
	/// the never-null arguments object, whose members are strings and include
	/// every required argument. Throwing is allowed: an MCPException becomes
	/// an error reply with its code and message; any other exception becomes
	/// an Internal error reply without its text, which the server logs (see
	/// Server::setLoggerName()).


} // namespace Poco::AI::MCP


#endif // AIMCP_Resource_INCLUDED
