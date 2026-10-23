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
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/Exception.h"
#include <sstream>
#include <string>


using namespace Poco::AI::MCP;


namespace
{
	template <typename F>
	void attempt(F f)
		/// Runs f and passes over what the API says it throws for bad input:
		/// an MCPException for an error reply or a message that is no JSON
		/// object, a ProtocolException or another IOException for a reply that
		/// breaks the protocol or never comes, and a DataFormatException from
		/// the parsers. Anything else is a finding.
	{
		try
		{
			f();
		}
		catch (const MCPException&)
		{
		}
		catch (const Poco::IOException&)
		{
		}
		catch (const Poco::DataFormatException&)
		{
		}
	}


	Poco::JSON::Object::Ptr parseObject(const std::string& input)
		/// The JSON object in input, or null. What the JSON parser throws for
		/// text that is no JSON, or holds a number out of range, is no finding
		/// of this target.
	{
		try
		{
			Poco::JSON::Parser parser;
			const Poco::Dynamic::Var parsed = parser.parse(input);
			if (parsed.type() == typeid(Poco::JSON::Object::Ptr)) return parsed.extract<Poco::JSON::Object::Ptr>();
		}
		catch (const Poco::JSON::JSONException&)
		{
		}
		catch (const Poco::SyntaxException&)
		{
		}
		return nullptr;
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	const std::string input(reinterpret_cast<const char*>(data), size);

	{
		std::istringstream in(input);
		std::ostringstream out;
		// A limit well below the inputs' length, so that long lines reach it.
		StreamClient client(in, out, 512);
		client.setMaxPages(4);
		attempt([&client]() { (void) client.discover(); });
		attempt([&client]() { (void) client.listTools(); });
		attempt([&client]() { (void) client.callTool("echo"); });
		attempt([&client]() { (void) client.readResource("fuzz://resource"); });
		attempt([&client]() { (void) client.getPrompt("prompt"); });
	}

	const Poco::JSON::Object::Ptr pObject = parseObject(input);
	if (pObject != nullptr)
	{
		attempt([&pObject]() { (void) ToolResult::fromObject(pObject).toObject(); });
		attempt([&pObject]() { (void) Tool::fromObject(pObject).toObject(); });
		attempt([&pObject]() { (void) Resource::fromObject(pObject).toObject(); });
		attempt([&pObject]() { (void) ResourceContents::fromObject(pObject).toObject(); });
		attempt([&pObject]() { (void) Prompt::fromObject(pObject).toObject(); });
		attempt([&pObject]() { (void) Implementation::fromObject(pObject).toObject(); });
	}
	return 0;
}
