//
// ServerFuzz.cpp
//
// Feeds arbitrary bytes to the stream ingress of an MCP server with a tool,
// a resource and a prompt.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/Server.h"
#include <sstream>
#include <string>


using namespace Poco::AI::MCP;


namespace
{
	Server& server()
	{
		static Server instance;
		static const bool ready = []()
		{
			Implementation info;
			info.name = "fuzz";
			info.version = "1";
			instance.setServerInfo(info);
			instance.registerTool("echo", "Echoes its message.", nullptr, [](const Poco::JSON::Object::Ptr& pArgs)
			{
				ToolResult result;
				result.addText(pArgs->optValue<std::string>("message", ""));
				return result;
			});
			Resource resource;
			resource.uri = "fuzz://resource";
			resource.name = "resource";
			instance.registerResource(resource, [](const std::string& uri, const RequestContext&)
			{
				ResourceContents contents;
				contents.uri = uri;
				contents.text = "text";
				return contents;
			});
			Prompt prompt;
			prompt.name = "prompt";
			PromptArgument argument;
			argument.name = "name";
			argument.required = true;
			prompt.arguments.push_back(argument);
			instance.registerPrompt(prompt, [](const Poco::JSON::Object::Ptr&, const RequestContext&)
			{
				return Poco::JSON::Array::Ptr(new Poco::JSON::Array);
			});
			return true;
		}();
		(void) ready;
		return instance;
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	std::istringstream in(std::string(reinterpret_cast<const char*>(data), size));
	std::ostringstream out;
	// serve() answers every message, valid or not, and throws only when out
	// fails, which a string stream does not: an exception is a finding. The
	// limit is well below the inputs' length, so that long lines reach it.
	server().serve(in, out, 512);
	return 0;
}
