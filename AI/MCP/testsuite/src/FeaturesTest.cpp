//
// FeaturesTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FeaturesTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/AI/MCP/Tool.h"
#include "Poco/AI/MCP/Resource.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/AI/MCP/Content.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"


using Poco::AI::MCP::Server;
using Poco::AI::MCP::Request;
using Poco::AI::MCP::Response;
using Poco::AI::MCP::Tool;
using Poco::AI::MCP::ToolResult;
using Poco::AI::MCP::Resource;
using Poco::AI::MCP::Prompt;
using Poco::AI::MCP::PromptArgument;
using Poco::AI::MCP::RequestContext;
using Poco::AI::MCP::Identity;
using Poco::AI::MCP::Id;
using Poco::JSON::Object;
using Poco::JSON::Array;


namespace
{
	void initialize(Server& server)
	{
		Object::Ptr params = new Object;
		params->set("protocolVersion", Poco::AI::MCP::PROTOCOL_VERSION);
		params->set("capabilities", Object::Ptr(new Object));
		server.handleMessage(Request::make(Id(1), "initialize", params));
	}

	Object::Ptr result(const Object::Ptr& response)
	{
		return response && response->isObject("result") ? response->getObject("result") : Object::Ptr();
	}
}


FeaturesTest::FeaturesTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


FeaturesTest::~FeaturesTest()
{
}


void FeaturesTest::testContextHandlerIdentity()
{
	Server server;
	server.setServerInfo("ctx-server", "1.0.0");

	// A context-aware tool that echoes back the caller's principal.
	Tool whoami;
	whoami.name = "whoami";
	whoami.description = "Returns the caller principal.";
	whoami.inputSchema = new Object;
	whoami.contextHandler = [](const Object::Ptr&, const RequestContext& ctx)
	{
		ToolResult r;
		r.addText(ctx.identity.principal);
		return r;
	};
	server.registerTool(whoami);
	initialize(server);

	Object::Ptr params = new Object;
	params->set("name", "whoami");
	params->set("arguments", Object::Ptr(new Object));

	RequestContext ctx;
	ctx.identity.principal = "alice";
	ctx.identity.authenticated = true;

	Object::Ptr response = server.handleMessage(Request::make(Id(2), "tools/call", params), ctx);
	Object::Ptr res = result(response);
	assertTrue(!res.isNull());
	Array::Ptr content = res->getArray("content");
	assertTrue(!content.isNull() && content->size() == 1);
	assertEqual(std::string("alice"), content->getObject(0)->getValue<std::string>("text"));
}


void FeaturesTest::testStructuredContent()
{
	ToolResult r;
	Object::Ptr structured = new Object;
	structured->set("rows", 3);
	r.addText("{\"rows\":3}");
	r.setStructuredContent(structured);

	Object::Ptr o = r.toObject();
	assertTrue(o->isObject("structuredContent"));
	assertEqual(3, o->getObject("structuredContent")->getValue<int>("rows"));
	// The text block is preserved for backwards compatibility.
	assertTrue(o->isArray("content") && o->getArray("content")->size() == 1);

	// Round-trips through fromObject.
	ToolResult parsed = ToolResult::fromObject(o);
	assertEqual(3, parsed.toObject()->getObject("structuredContent")->getValue<int>("rows"));
}


void FeaturesTest::testResources()
{
	Server server;
	server.setServerInfo("res-server", "1.0.0");

	Resource doc;
	doc.uri = "test://doc";
	doc.name = "Doc";
	doc.description = "A test document";
	doc.mimeType = "text/markdown";
	server.registerResource(doc, [](const std::string& uri) { return "# " + uri; });
	initialize(server);

	// initialize advertises the resources capability now that one is registered.
	Object::Ptr init = result(server.handleMessage(Request::make(Id(9), "initialize",
		Object::Ptr(new Object))));
	assertTrue(init->getObject("capabilities")->isObject("resources"));

	// resources/list
	Object::Ptr list = result(server.handleMessage(Request::make(Id(2), "resources/list")));
	Array::Ptr resources = list->getArray("resources");
	assertTrue(!resources.isNull() && resources->size() == 1);
	assertEqual(std::string("test://doc"), resources->getObject(0)->getValue<std::string>("uri"));

	// resources/read
	Object::Ptr params = new Object;
	params->set("uri", "test://doc");
	Object::Ptr read = result(server.handleMessage(Request::make(Id(3), "resources/read", params)));
	Array::Ptr contents = read->getArray("contents");
	assertTrue(!contents.isNull() && contents->size() == 1);
	assertEqual(std::string("# test://doc"), contents->getObject(0)->getValue<std::string>("text"));

	// Unknown resource is a JSON-RPC error.
	Object::Ptr bad = new Object;
	bad->set("uri", "test://missing");
	Object::Ptr err = server.handleMessage(Request::make(Id(4), "resources/read", bad));
	assertTrue(err->isObject("error"));
}


void FeaturesTest::testPrompts()
{
	Server server;
	server.setServerInfo("prompt-server", "1.0.0");

	Prompt p;
	p.name = "greet";
	p.description = "Greet someone";
	p.arguments.push_back({"who", "Who to greet", true});
	server.registerPrompt(p, [](const Object::Ptr& args)
	{
		Object::Ptr content = new Object;
		content->set("type", "text");
		content->set("text", "Hello " + args->optValue<std::string>("who", "world"));
		Object::Ptr msg = new Object;
		msg->set("role", "user");
		msg->set("content", content);
		Array::Ptr messages = new Array;
		messages->add(msg);
		return messages;
	});
	initialize(server);

	// prompts/list includes the declared argument.
	Object::Ptr list = result(server.handleMessage(Request::make(Id(2), "prompts/list")));
	Array::Ptr prompts = list->getArray("prompts");
	assertTrue(!prompts.isNull() && prompts->size() == 1);
	Array::Ptr args = prompts->getObject(0)->getArray("arguments");
	assertTrue(!args.isNull() && args->size() == 1);
	assertEqual(std::string("who"), args->getObject(0)->getValue<std::string>("name"));

	// prompts/get renders with the supplied argument.
	Object::Ptr getParams = new Object;
	getParams->set("name", "greet");
	Object::Ptr argObj = new Object;
	argObj->set("who", "World");
	getParams->set("arguments", argObj);
	Object::Ptr got = result(server.handleMessage(Request::make(Id(3), "prompts/get", getParams)));
	Array::Ptr messages = got->getArray("messages");
	assertTrue(!messages.isNull() && messages->size() == 1);
	assertEqual(std::string("Hello World"),
		messages->getObject(0)->getObject("content")->getValue<std::string>("text"));
}


void FeaturesTest::testToolsOnlyHasNoResourceCaps()
{
	// A tools-only server must not advertise resources/prompts capabilities, so
	// existing tools-only servers are unaffected by the new registries.
	Server server;
	server.setServerInfo("tools-only", "1.0.0");
	server.registerTool("noop", "Does nothing.", new Object,
		[](Object::Ptr) { return ToolResult(); });

	Object::Ptr init = result(server.handleMessage(Request::make(Id(1), "initialize",
		Object::Ptr(new Object))));
	Object::Ptr caps = init->getObject("capabilities");
	assertTrue(caps->isObject("tools"));
	assertTrue(!caps->has("resources"));
	assertTrue(!caps->has("prompts"));
}


void FeaturesTest::setUp()
{
}


void FeaturesTest::tearDown()
{
}


CppUnit::Test* FeaturesTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FeaturesTest");

	CppUnit_addTest(pSuite, FeaturesTest, testContextHandlerIdentity);
	CppUnit_addTest(pSuite, FeaturesTest, testStructuredContent);
	CppUnit_addTest(pSuite, FeaturesTest, testResources);
	CppUnit_addTest(pSuite, FeaturesTest, testPrompts);
	CppUnit_addTest(pSuite, FeaturesTest, testToolsOnlyHasNoResourceCaps);

	return pSuite;
}
