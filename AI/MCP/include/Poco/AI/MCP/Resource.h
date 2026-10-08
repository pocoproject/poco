//
// Resource.h
//
// Library: AIMCP
// Package: Core
// Module:  Resource
//
// Descriptors and handler callbacks for the MCP resources/* and prompts/* method
// families. A server registers resources (readable context documents) and prompts
// (reusable, optionally parameterized message templates); the Dispatcher exposes
// them via resources/list, resources/read, prompts/list and prompts/get.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCP_Resource_INCLUDED
#define AIMCP_Resource_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include <functional>
#include <string>
#include <vector>


namespace Poco {
namespace AI {
namespace MCP {


struct AIMCP_API Resource
	/// Describes a resource exposed via resources/list and resources/read.
{
	std::string uri;
	std::string name;
	std::string description;
	std::string mimeType;
};


using ResourceReader = std::function<std::string (const std::string& uri)>;
	/// Returns the textual contents of the resource identified by uri. Throwing is
	/// allowed - the Dispatcher turns it into a JSON-RPC error reply.


struct AIMCP_API PromptArgument
	/// A declared argument a prompt accepts (advertised by prompts/list).
{
	std::string name;
	std::string description;
	bool required = false;
};


struct AIMCP_API Prompt
	/// Describes a prompt exposed via prompts/list and prompts/get.
{
	std::string name;
	std::string description;
	std::vector<PromptArgument> arguments;
};


using PromptRenderer = std::function<Poco::JSON::Array::Ptr (const Poco::JSON::Object::Ptr& arguments)>;
	/// Renders a prompt into the messages array returned by prompts/get. Receives
	/// the (never-null) caller-supplied arguments object.


} } } // namespace Poco::AI::MCP


#endif // AIMCP_Resource_INCLUDED
