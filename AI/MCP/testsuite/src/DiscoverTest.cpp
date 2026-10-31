//
// DiscoverTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DiscoverTest.h"
#include "MCPTestHelpers.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Parser.h"


using Poco::AI::MCP::Server;
using Poco::AI::MCP::Request;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::Resource;
using Poco::AI::MCP::ResourceContents;
using Poco::AI::MCP::Prompt;
using Poco::AI::MCP::RequestContext;
using Poco::AI::MCP::Implementation;
using Poco::AI::MCP::ErrorCode;
using Poco::AI::MCP::Id;
using Poco::JSON::Object;
using Poco::JSON::Array;
using MCPTest::request;
using MCPTest::errorCode;
using MCPTest::result;
using MCPTest::code;


namespace
{
	void addEcho(Server& server)
	{
		server.registerTool("echo", "Echo back the message.", nullptr,
			[](const Object::Ptr& pArgs)
			{
				ToolResult toolResult;
				toolResult.addText(pArgs->optValue<std::string>("message", ""));
				return toolResult;
			});
	}


	void addResourceAndPrompt(Server& server)
		/// A resource and a prompt, so that their capabilities are advertised.
	{
		Poco::AI::MCP::Resource resource;
		resource.uri = "doc://readme";
		resource.name = "readme";
		server.registerResource(resource, [](const std::string& uri, const Poco::AI::MCP::RequestContext&)
		{
			Poco::AI::MCP::ResourceContents contents;
			contents.uri = uri;
			contents.text = "read me";
			return contents;
		});
		Poco::AI::MCP::Prompt prompt;
		prompt.name = "greet";
		server.registerPrompt(prompt, [](const Object::Ptr&, const Poco::AI::MCP::RequestContext&)
		{
			return Poco::JSON::Array::Ptr(new Poco::JSON::Array);
		});
	}


	Implementation info(const std::string& name)
	{
		Implementation impl;
		impl.name = name;
		impl.version = "1.2.3";
		return impl;
	}


	Object::Ptr parse(const std::string& json)
	{
		Poco::JSON::Parser parser;
		return parser.parse(json).extract<Object::Ptr>();
	}


	Object::Ptr metaWithout(const std::string& key)
	{
		Object::Ptr pMeta = MCPTest::meta();
		pMeta->remove(key);
		return pMeta;
	}
}


DiscoverTest::DiscoverTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


DiscoverTest::~DiscoverTest()
{
}


void DiscoverTest::testDiscover()
{
	Server server;
	server.setServerInfo(info("discover-test"));
	server.setInstructions("Use echo.");
	addEcho(server);

	Object::Ptr pReply = server.handle(request(Id(1), "server/discover"));
	assertEqual(1, pReply->getValue<int>("id"));
	Object::Ptr pResult = result(pReply);
	assertTrue(!pResult.isNull());
	assertEqual(std::string("complete"), pResult->getValue<std::string>("resultType"));
	Array::Ptr pVersions = pResult->getArray("supportedVersions");
	assertEqual(1u, static_cast<unsigned>(pVersions->size()));
	assertEqual(std::string(Poco::AI::MCP::PROTOCOL_VERSION), pVersions->getElement<std::string>(0));
	assertEqual(std::string("Use echo."), pResult->getValue<std::string>("instructions"));
	Object::Ptr pServerInfo = pResult->getObject("_meta")->getObject(Poco::AI::MCP::META_SERVER_INFO);
	assertEqual(std::string("discover-test"), pServerInfo->getValue<std::string>("name"));
	assertEqual(std::string("1.2.3"), pServerInfo->getValue<std::string>("version"));
	// No serverInfo at the top level: it lives in _meta.
	assertTrue(!pResult->has("serverInfo"));
	assertTrue(!pResult->has("protocolVersion"));
}


void DiscoverTest::testDiscoverCapabilities()
{
	Server toolsOnly;
	addEcho(toolsOnly);
	Object::Ptr pCapabilities = result(toolsOnly.handle(request(Id(1), "server/discover")))->getObject("capabilities");
	assertTrue(pCapabilities->isObject("tools"));
	assertTrue(!pCapabilities->has("resources"));
	assertTrue(!pCapabilities->has("prompts"));
	assertTrue(!pCapabilities->has("logging"));

	Server full;
	addEcho(full);
	Resource resource;
	resource.uri = "test://r";
	resource.name = "r";
	full.registerResource(resource, [](const std::string& uri, const RequestContext&) { ResourceContents c; c.uri = uri; c.text = "x"; return c; });
	Prompt prompt;
	prompt.name = "p";
	full.registerPrompt(prompt, [](const Object::Ptr&, const RequestContext&) { return Array::Ptr(new Array); });
	pCapabilities = result(full.handle(request(Id(2), "server/discover")))->getObject("capabilities");
	assertTrue(pCapabilities->isObject("tools"));
	assertTrue(pCapabilities->isObject("resources"));
	assertTrue(pCapabilities->isObject("prompts"));
}


void DiscoverTest::testServerInfoOnEveryResult()
{
	Server server;
	addEcho(server);
	addResourceAndPrompt(server);

	// By default the server reports this library.
	Object::Ptr pResult = result(server.handle(request(Id(1), "tools/list")));
	assertEqual(std::string("complete"), pResult->getValue<std::string>("resultType"));
	Object::Ptr pLibrary = pResult->getObject("_meta")->getObject(Poco::AI::MCP::META_SERVER_INFO);
	assertEqual(Implementation::library().name, pLibrary->getValue<std::string>("name"));
	assertEqual(Implementation::library().version, pLibrary->getValue<std::string>("version"));
	assertTrue(!Implementation::library().version.empty());

	// Told not to, results carry no _meta.
	server.setServerInfo(Implementation());
	pResult = result(server.handle(request(Id(2), "tools/list")));
	assertTrue(!pResult->has("_meta"));

	server.setServerInfo(info("everywhere"));
	const char* methods[] = {"server/discover", "tools/list", "resources/list", "resources/templates/list", "prompts/list"};
	int id = 10;
	for (const char* method: methods)
	{
		pResult = result(server.handle(request(Id(id++), method)));
		assertTrue(!pResult.isNull());
		assertEqual(std::string("complete"), pResult->getValue<std::string>("resultType"));
		assertEqual(std::string("everywhere"), pResult->getObject("_meta")->getObject(Poco::AI::MCP::META_SERVER_INFO)->getValue<std::string>("name"));
	}

	Object::Ptr pParams = new Object;
	pParams->set("name", "echo");
	pResult = result(server.handle(request(Id(id), "tools/call", pParams)));
	assertEqual(std::string("complete"), pResult->getValue<std::string>("resultType"));
	assertEqual(std::string("everywhere"), pResult->getObject("_meta")->getObject(Poco::AI::MCP::META_SERVER_INFO)->getValue<std::string>("name"));
}


void DiscoverTest::testCacheHints()
{
	Server server;
	addEcho(server);
	addResourceAndPrompt(server);

	Object::Ptr pResult = result(server.handle(request(Id(1), "tools/list")));
	assertEqual(0, pResult->getValue<int>("ttlMs"));
	assertEqual(std::string("private"), pResult->getValue<std::string>("cacheScope"));

	server.setCacheHints(Poco::Timespan(90, 0), Server::CacheScope::PUBLIC);
	const char* methods[] = {"server/discover", "tools/list", "resources/list", "resources/templates/list", "prompts/list"};
	int id = 10;
	for (const char* method: methods)
	{
		pResult = result(server.handle(request(Id(id++), method)));
		assertEqual(90000, pResult->getValue<int>("ttlMs"));
		assertEqual(std::string("public"), pResult->getValue<std::string>("cacheScope"));
	}

	// A tool call is not cacheable.
	Object::Ptr pParams = new Object;
	pParams->set("name", "echo");
	pResult = result(server.handle(request(Id(id), "tools/call", pParams)));
	assertTrue(!pResult->has("ttlMs"));
	assertTrue(!pResult->has("cacheScope"));
}


void DiscoverTest::testMissingMeta()
{
	Server server;
	addEcho(server);

	// No params at all.
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(server.handle(Request::make(Id(1), "tools/list"))));

	// Params without _meta.
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(server.handle(Request::make(Id(2), "tools/list", Object::Ptr(new Object)))));

	// A _meta object that is not an object.
	Object::Ptr pParams = new Object;
	pParams->set("_meta", "2026-07-28");
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(server.handle(Request::make(Id(3), "tools/list", pParams))));

	// _meta without the protocol version.
	pParams = new Object;
	pParams->set("_meta", metaWithout(Poco::AI::MCP::META_PROTOCOL_VERSION));
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(server.handle(Request::make(Id(4), "tools/list", pParams))));

	// _meta without the client capabilities.
	pParams = new Object;
	pParams->set("_meta", metaWithout(Poco::AI::MCP::META_CLIENT_CAPABILITIES));
	Object::Ptr pReply = server.handle(Request::make(Id(5), "tools/list", pParams));
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(pReply));
	assertEqual(5, pReply->getValue<int>("id"));

	// Params that are not an object.
	Object::Ptr pRequest = parse("{\"jsonrpc\":\"2.0\",\"id\":6,\"method\":\"tools/list\",\"params\":[1]}");
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(server.handle(pRequest)));
}


void DiscoverTest::testUnsupportedVersion()
{
	Server server;
	addEcho(server);

	Object::Ptr pMeta = MCPTest::meta();
	pMeta->set(Poco::AI::MCP::META_PROTOCOL_VERSION, "2025-06-18");
	Object::Ptr pParams = new Object;
	pParams->set("_meta", pMeta);
	Object::Ptr pReply = server.handle(Request::make(Id(1), "tools/list", pParams));
	assertEqual(code(ErrorCode::UNSUPPORTED_PROTOCOL_VERSION), errorCode(pReply));
	Object::Ptr pData = pReply->getObject("error")->getObject("data");
	assertEqual(std::string(Poco::AI::MCP::PROTOCOL_VERSION), pData->getArray("supported")->getElement<std::string>(0));
	assertEqual(std::string("2025-06-18"), pData->getValue<std::string>("requested"));

	// The version is checked before the capabilities.
	pMeta = metaWithout(Poco::AI::MCP::META_CLIENT_CAPABILITIES);
	pMeta->set(Poco::AI::MCP::META_PROTOCOL_VERSION, "1900-01-01");
	pParams = new Object;
	pParams->set("_meta", pMeta);
	assertEqual(code(ErrorCode::UNSUPPORTED_PROTOCOL_VERSION), errorCode(server.handle(Request::make(Id(2), "tools/list", pParams))));
}


void DiscoverTest::testInitializeRefused()
{
	Server server;
	addEcho(server);

	Object::Ptr pRequest = parse("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\",\"params\":{\"protocolVersion\":\"2025-06-18\",\"capabilities\":{},\"clientInfo\":{\"name\":\"old\",\"version\":\"1\"}}}");
	Object::Ptr pReply = server.handle(pRequest);
	assertEqual(1, pReply->getValue<int>("id"));
	assertEqual(code(ErrorCode::UNSUPPORTED_PROTOCOL_VERSION), errorCode(pReply));
	Object::Ptr pData = pReply->getObject("error")->getObject("data");
	assertEqual(std::string(Poco::AI::MCP::PROTOCOL_VERSION), pData->getArray("supported")->getElement<std::string>(0));
	assertEqual(std::string("2025-06-18"), pData->getValue<std::string>("requested"));
	// The message names the revision too, for a client that shows only it.
	assertTrue(pReply->getObject("error")->getValue<std::string>("message").find(Poco::AI::MCP::PROTOCOL_VERSION) != std::string::npos);

	// An initialize that names no revision is told the same, with an empty
	// requested revision.
	pReply = server.handle(parse("{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"initialize\",\"params\":{}}"));
	assertEqual(code(ErrorCode::UNSUPPORTED_PROTOCOL_VERSION), errorCode(pReply));
	pData = pReply->getObject("error")->getObject("data");
	assertTrue(pData->has("requested"));
	assertEqual(std::string(""), pData->getValue<std::string>("requested"));

	// ping went with the handshake.
	assertEqual(code(ErrorCode::METHOD_NOT_FOUND), errorCode(server.handle(request(Id(2), "ping"))));
}


void DiscoverTest::testInvalidIds()
{
	Server server;
	addEcho(server);

	const char* requests[] = {
		"{\"jsonrpc\":\"2.0\",\"id\":null,\"method\":\"tools/list\",\"params\":{}}",
		"{\"jsonrpc\":\"2.0\",\"id\":true,\"method\":\"tools/list\",\"params\":{}}",
		"{\"jsonrpc\":\"2.0\",\"id\":1.5,\"method\":\"tools/list\",\"params\":{}}",
		"{\"jsonrpc\":\"2.0\",\"id\":{},\"method\":\"tools/list\",\"params\":{}}",
		"{\"jsonrpc\":\"2.0\",\"id\":[],\"method\":\"tools/list\",\"params\":{}}"
	};
	for (const char* text: requests)
	{
		Object::Ptr pReply = server.handle(parse(text));
		assertTrue(!pReply.isNull());
		assertEqual(code(ErrorCode::INVALID_REQUEST), errorCode(pReply));
		assertTrue(!pReply->has("id"));
	}

	// String ids are echoed as strings.
	Object::Ptr pReply = server.handle(request(Id(std::string("abc")), "tools/list"));
	assertEqual(std::string("abc"), pReply->getValue<std::string>("id"));
}


void DiscoverTest::testNotificationsGetNoReply()
{
	Server server;
	addEcho(server);

	Object::Ptr pParams = new Object;
	pParams->set("requestId", 5);
	assertTrue(server.handle(Request::makeNotification("notifications/cancelled", pParams)).isNull());
	assertTrue(server.handle(Request::makeNotification("notifications/initialized")).isNull());
	assertTrue(server.handle(Request::makeNotification("anything/else")).isNull());
	// A message without an id that is not a valid notification is an
	// invalid request, answered without an id.
	const char* malformed[] = {
		"{\"jsonrpc\":\"2.0\",\"method\":42}",
		"{\"jsonrpc\":\"1.0\",\"method\":\"tools/list\"}",
		"{\"method\":\"tools/list\"}",
		"{\"jsonrpc\":\"2.0\"}"
	};
	for (const char* text: malformed)
	{
		Object::Ptr pReply = server.handle(parse(text));
		assertTrue(!pReply.isNull());
		assertEqual(code(ErrorCode::INVALID_REQUEST), errorCode(pReply));
		assertTrue(!pReply->has("id"));
	}
	// So is a malformed request, with its id.
	Object::Ptr pReply = server.handle(parse("{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":42}"));
	assertEqual(code(ErrorCode::INVALID_REQUEST), errorCode(pReply));
	assertEqual(3, pReply->getValue<int>("id"));
	assertEqual(code(ErrorCode::INVALID_REQUEST), errorCode(server.handle(Object::Ptr())));
}


void DiscoverTest::testUnknownMethod()
{
	Server server;
	addEcho(server);
	Object::Ptr pReply = server.handle(request(Id(9), "subscriptions/listen"));
	assertEqual(code(ErrorCode::METHOD_NOT_FOUND), errorCode(pReply));
	assertEqual(9, pReply->getValue<int>("id"));
}


void DiscoverTest::testUnadvertisedCapabilities()
{
	// The methods of a capability server/discover does not advertise are
	// not found.
	Server server;
	addEcho(server);
	Object::Ptr pCapabilities = result(server.handle(request(Id(1), "server/discover")))->getObject("capabilities");
	assertTrue(!pCapabilities->has("resources"));
	assertTrue(!pCapabilities->has("prompts"));
	Object::Ptr pRead = new Object;
	pRead->set("uri", "doc://readme");
	Object::Ptr pGet = new Object;
	pGet->set("name", "greet");
	assertEqual(code(ErrorCode::METHOD_NOT_FOUND), errorCode(server.handle(request(Id(2), "resources/list"))));
	assertEqual(code(ErrorCode::METHOD_NOT_FOUND), errorCode(server.handle(request(Id(3), "resources/templates/list"))));
	assertEqual(code(ErrorCode::METHOD_NOT_FOUND), errorCode(server.handle(request(Id(4), "resources/read", pRead))));
	assertEqual(code(ErrorCode::METHOD_NOT_FOUND), errorCode(server.handle(request(Id(5), "prompts/list"))));
	assertEqual(code(ErrorCode::METHOD_NOT_FOUND), errorCode(server.handle(request(Id(6), "prompts/get", pGet))));

	addResourceAndPrompt(server);
	pCapabilities = result(server.handle(request(Id(7), "server/discover")))->getObject("capabilities");
	assertTrue(pCapabilities->has("resources"));
	assertTrue(pCapabilities->has("prompts"));
	assertTrue(!result(server.handle(request(Id(8), "resources/list"))).isNull());
	assertTrue(!result(server.handle(request(Id(9), "resources/templates/list"))).isNull());
	assertTrue(!result(server.handle(request(Id(10), "resources/read", pRead))).isNull());
	assertTrue(!result(server.handle(request(Id(11), "prompts/list"))).isNull());
	assertTrue(!result(server.handle(request(Id(12), "prompts/get", pGet))).isNull());
}


void DiscoverTest::testContextFromMeta()
{
	Server server;
	RequestContext seen;
	Poco::AI::MCP::Tool tool;
	tool.name = "who";
	tool.handler = [&seen](const Object::Ptr&, const RequestContext& context)
	{
		seen = context;
		return ToolResult();
	};
	server.registerTool(tool);

	Object::Ptr pMeta = MCPTest::meta();
	Object::Ptr pCapabilities = new Object;
	pCapabilities->set("elicitation", Object::Ptr(new Object));
	pMeta->set(Poco::AI::MCP::META_CLIENT_CAPABILITIES, pCapabilities);
	pMeta->set(Poco::AI::MCP::META_CLIENT_INFO, info("caller").toObject());
	Object::Ptr pParams = new Object;
	pParams->set("name", "who");
	pParams->set("_meta", pMeta);

	RequestContext context;
	context.identity.principal = "alice";
	context.identity.authenticated = true;
	Object::Ptr pReply = server.handle(Request::make(Id(1), "tools/call", pParams), context);
	assertEqual(0, errorCode(pReply));
	assertEqual(std::string("alice"), seen.identity.principal);
	assertTrue(seen.identity.authenticated);
	assertEqual(std::string(Poco::AI::MCP::PROTOCOL_VERSION), seen.protocolVersion);
	assertEqual(std::string("caller"), seen.clientInfo.name);
	assertTrue(!seen.clientCapabilities.isNull());
	assertTrue(seen.clientCapabilities->has("elicitation"));
}


void DiscoverTest::setUp()
{
}


void DiscoverTest::tearDown()
{
}


CppUnit::Test* DiscoverTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DiscoverTest");

	CppUnit_addTest(pSuite, DiscoverTest, testDiscover);
	CppUnit_addTest(pSuite, DiscoverTest, testDiscoverCapabilities);
	CppUnit_addTest(pSuite, DiscoverTest, testServerInfoOnEveryResult);
	CppUnit_addTest(pSuite, DiscoverTest, testCacheHints);
	CppUnit_addTest(pSuite, DiscoverTest, testMissingMeta);
	CppUnit_addTest(pSuite, DiscoverTest, testUnsupportedVersion);
	CppUnit_addTest(pSuite, DiscoverTest, testInitializeRefused);
	CppUnit_addTest(pSuite, DiscoverTest, testInvalidIds);
	CppUnit_addTest(pSuite, DiscoverTest, testNotificationsGetNoReply);
	CppUnit_addTest(pSuite, DiscoverTest, testUnknownMethod);
	CppUnit_addTest(pSuite, DiscoverTest, testUnadvertisedCapabilities);
	CppUnit_addTest(pSuite, DiscoverTest, testContextFromMeta);

	return pSuite;
}
