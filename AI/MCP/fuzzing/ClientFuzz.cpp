//
// ClientFuzz.cpp
//
// Feeds arbitrary bytes as the replies of a server to a stream client, and
// as JSON to the parsers of tools, tool results, resources and prompts.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/StreamClient.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Exception.h"
#include <exception>
#include <sstream>
#include <string>


using namespace Poco::AI::MCP;


namespace
{
	template <typename F>
	void attempt(F f)
	{
		try
		{
			f();
		}
		catch (const Poco::Exception&)
		{
		}
		catch (const std::exception&)
		{
		}
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	const std::string input(reinterpret_cast<const char*>(data), size);

	{
		std::istringstream in(input);
		std::ostringstream out;
		StreamClient client(in, out, 65536);
		client.setMaxPages(4);
		attempt([&client]() { (void) client.discover(); });
		attempt([&client]() { (void) client.listTools(); });
		attempt([&client]() { (void) client.callTool("echo"); });
		attempt([&client]() { (void) client.readResource("fuzz://resource"); });
		attempt([&client]() { (void) client.getPrompt("prompt"); });
	}

	attempt([&input]()
	{
		Poco::JSON::Parser parser;
		const Poco::Dynamic::Var parsed = parser.parse(input);
		if (parsed.type() != typeid(Poco::JSON::Object::Ptr)) return;
		const Poco::JSON::Object::Ptr pObject = parsed.extract<Poco::JSON::Object::Ptr>();
		attempt([&pObject]() { (void) ToolResult::fromObject(pObject).toObject(); });
		attempt([&pObject]() { (void) Tool::fromObject(pObject).toObject(); });
		attempt([&pObject]() { (void) Resource::fromObject(pObject).toObject(); });
		attempt([&pObject]() { (void) ResourceContents::fromObject(pObject).toObject(); });
		attempt([&pObject]() { (void) Prompt::fromObject(pObject).toObject(); });
		attempt([&pObject]() { (void) Implementation::fromObject(pObject).toObject(); });
	});
	return 0;
}
