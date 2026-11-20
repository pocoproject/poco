//
// ClientFuzz.cpp
//
// Feeds arbitrary bytes as the replies of a server to a stream client, and
// as JSON to the parsers of tools, tool results, resources and prompts,
// whose results must convert to JSON and back without a change.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/MCP/StreamClient.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/AI/MCP/Implementation.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/AI/MCP/Resource.h"
#include "Poco/AI/MCP/Tool.h"
#include "Poco/AI/MCP/ToolResult.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>


using namespace Poco::AI::MCP;


namespace
{
	template <typename F>
	void attempt(F f)
		/// Runs f and passes over what a request throws for a reply that is
		/// an error, malformed or missing: an MCPException for an error reply
		/// or a server that does not implement the protocol revision, and a
		/// ProtocolException or another IOException for a reply that breaks
		/// the protocol or never comes. Serves discover(), listTools() and
		/// getPrompt(), which throw nothing else: listTools() passes over a
		/// tool that is not valid. Anything else is a finding.
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
	}


	template <typename F>
	void attemptParsing(F f)
		/// Runs f as attempt() does, and also passes over the
		/// DataFormatException of the parsers of a tool result and of resource
		/// contents, which callTool() and readResource() let through for a
		/// result that is not valid. Anything else is a finding.
	{
		try
		{
			attempt(f);
		}
		catch (const Poco::DataFormatException&)
		{
		}
	}


	template <typename T>
	void checkRoundTrip(const Poco::JSON::Object::Ptr& pObject)
		/// Reads a T from pObject, which is passed over when it is not a valid
		/// T: fromObject() throws a DataFormatException for it. The object
		/// toObject() gives for an accepted T is accepted again and converts
		/// to the same object, with the same keys in the same order and the
		/// same values. Anything else, a difference or an exception, is a
		/// finding.
	{
		T value;
		try
		{
			value = T::fromObject(pObject);
		}
		catch (const Poco::DataFormatException&)
		{
			return;
		}
		const Poco::JSON::Object::Ptr pFirst = value.toObject();
		const Poco::JSON::Object::Ptr pSecond = T::fromObject(pFirst).toObject();
		const std::string first = serialize(pFirst);
		const std::string second = serialize(pSecond);
		if (first != second)
		{
			std::cerr << "The round trip changes " << first << " into " << second << '\n';
			std::abort();
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
		attemptParsing([&client]() { (void) client.callTool("echo"); });
		attemptParsing([&client]() { (void) client.readResource("fuzz://resource"); });
		attempt([&client]() { (void) client.getPrompt("prompt"); });
	}

	const Poco::JSON::Object::Ptr pObject = parseObject(input);
	if (pObject != nullptr)
	{
		checkRoundTrip<ToolResult>(pObject);
		checkRoundTrip<Tool>(pObject);
		checkRoundTrip<Resource>(pObject);
		checkRoundTrip<ResourceContents>(pObject);
		checkRoundTrip<Prompt>(pObject);
		checkRoundTrip<Implementation>(pObject);
	}
	return 0;
}
