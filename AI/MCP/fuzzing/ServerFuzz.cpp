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
#include "Poco/Exception.h"
#include <exception>
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
	try
	{
		server().serve(in, out, 65536);
	}
	catch (const Poco::Exception&)
	{
	}
	catch (const std::exception&)
	{
	}
	return 0;
}
