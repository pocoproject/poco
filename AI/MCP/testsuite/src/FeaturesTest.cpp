//
// FeaturesTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FeaturesTest.h"
#include "MCPTestHelpers.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/Server.h"
#include "Poco/AI/MCP/Resource.h"
#include "Poco/AI/MCP/RequestContext.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Array.h"
#include "Poco/Exception.h"


using Poco::AI::MCP::Server;
using Poco::AI::MCP::Resource;
using Poco::AI::MCP::ResourceContents;
using Poco::AI::MCP::Prompt;
using Poco::AI::MCP::PromptArgument;
using Poco::AI::MCP::RequestContext;
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
	void buildServer(Server& server)
	{
		Resource readme;
		readme.uri = "test://readme";
		readme.name = "readme";
		readme.title = "Read me";
		readme.description = "The read-me text.";
		readme.mimeType = "text/plain";
		server.registerResource(readme, [](const std::string&, const RequestContext& context)
		{
			ResourceContents contents;
			contents.text = "Hello, " + (context.clientInfo.empty() ? std::string("stranger") : context.clientInfo.name);
			return contents;
		});

		Resource logo;
		logo.uri = "test://logo";
		logo.name = "logo";
		logo.mimeType = "image/png";
		server.registerResource(logo, [](const std::string& uri, const RequestContext&)
		{
			ResourceContents contents;
			contents.uri = uri;
			contents.blob = "iVBORw0KGgo=";
			return contents;
		});

		Resource broken;
		broken.uri = "test://broken";
		broken.name = "broken";
		server.registerResource(broken, [](const std::string&, const RequestContext&) -> ResourceContents
		{
			throw Poco::IOException("disk on fire");
		});

		Prompt greet;
		greet.name = "greet";
		greet.title = "Greeting";
		greet.description = "Greets someone.";
		PromptArgument who;
		who.name = "who";
		who.description = "Whom to greet.";
		who.required = true;
		PromptArgument mood;
		mood.name = "mood";
		greet.arguments = {who, mood};
		server.registerPrompt(greet, [](const Object::Ptr& pArgs, const RequestContext&)
		{
			Object::Ptr pText = new Object;
			pText->set("type", "text");
			pText->set("text", "Hello " + pArgs->getValue<std::string>("who") + " " + pArgs->optValue<std::string>("mood", "plainly"));
			Object::Ptr pMessage = new Object;
			pMessage->set("role", "user");
			pMessage->set("content", pText);
			Array::Ptr pMessages = new Array;
			pMessages->add(pMessage);
			return pMessages;
		});
	}


	Object::Ptr read(Server& server, int id, const Poco::Dynamic::Var& uri)
	{
		Object::Ptr pParams = new Object;
		pParams->set("uri", uri);
		return server.handle(request(Id(id), "resources/read", pParams));
	}


	Object::Ptr getPrompt(Server& server, int id, const std::string& name, const Object::Ptr& pArgs)
	{
		Object::Ptr pParams = new Object;
		pParams->set("name", name);
		if (pArgs != nullptr) pParams->set("arguments", pArgs);
		return server.handle(request(Id(id), "prompts/get", pParams));
	}
}


FeaturesTest::FeaturesTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


FeaturesTest::~FeaturesTest()
{
}


void FeaturesTest::testResourcesList()
{
	Server server;
	buildServer(server);
	Array::Ptr pResources = result(server.handle(request(Id(1), "resources/list")))->getArray("resources");
	assertEqual(3u, static_cast<unsigned>(pResources->size()));
	// In URI order.
	assertEqual(std::string("test://broken"), pResources->getObject(0)->getValue<std::string>("uri"));
	assertEqual(std::string("test://logo"), pResources->getObject(1)->getValue<std::string>("uri"));
	Object::Ptr pReadme = pResources->getObject(2);
	assertEqual(std::string("readme"), pReadme->getValue<std::string>("name"));
	assertEqual(std::string("Read me"), pReadme->getValue<std::string>("title"));
	assertEqual(std::string("text/plain"), pReadme->getValue<std::string>("mimeType"));
}


void FeaturesTest::testResourceReadText()
{
	Server server;
	buildServer(server);
	Object::Ptr pResult = result(read(server, 1, std::string("test://readme")));
	assertEqual(std::string("complete"), pResult->getValue<std::string>("resultType"));
	assertTrue(pResult->has("ttlMs"));
	Object::Ptr pContents = pResult->getArray("contents")->getObject(0);
	// The URI and the media type default to those of the registration.
	assertEqual(std::string("test://readme"), pContents->getValue<std::string>("uri"));
	assertEqual(std::string("text/plain"), pContents->getValue<std::string>("mimeType"));
	assertEqual(std::string("Hello, stranger"), pContents->getValue<std::string>("text"));
	assertTrue(!pContents->has("blob"));
}


void FeaturesTest::testResourceReadBlob()
{
	Server server;
	buildServer(server);
	Object::Ptr pContents = result(read(server, 1, std::string("test://logo")))->getArray("contents")->getObject(0);
	assertEqual(std::string("iVBORw0KGgo="), pContents->getValue<std::string>("blob"));
	assertEqual(std::string("image/png"), pContents->getValue<std::string>("mimeType"));
	assertTrue(!pContents->has("text"));
}


void FeaturesTest::testResourceNotFound()
{
	Server server;
	buildServer(server);
	Object::Ptr pReply = read(server, 7, std::string("test://missing"));
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(pReply));
	assertEqual(7, pReply->getValue<int>("id"));
	assertEqual(std::string("test://missing"), pReply->getObject("error")->getObject("data")->getValue<std::string>("uri"));

	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(read(server, 8, 5)));
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(server.handle(request(Id(9), "resources/read"))));
}


void FeaturesTest::testResourceReaderThrows()
{
	Server server;
	buildServer(server);
	Object::Ptr pReply = read(server, 3, std::string("test://broken"));
	assertEqual(code(ErrorCode::INTERNAL_ERROR), errorCode(pReply));
	assertEqual(3, pReply->getValue<int>("id"));
	assertTrue(pReply->getObject("error")->getValue<std::string>("message").find("disk on fire") != std::string::npos);
}


void FeaturesTest::testResourceTemplatesEmpty()
{
	Server server;
	buildServer(server);
	Object::Ptr pResult = result(server.handle(request(Id(1), "resources/templates/list")));
	assertEqual(0u, static_cast<unsigned>(pResult->getArray("resourceTemplates")->size()));
	assertTrue(pResult->has("cacheScope"));
}


void FeaturesTest::testPromptsList()
{
	Server server;
	buildServer(server);
	Array::Ptr pPrompts = result(server.handle(request(Id(1), "prompts/list")))->getArray("prompts");
	assertEqual(1u, static_cast<unsigned>(pPrompts->size()));
	Object::Ptr pGreet = pPrompts->getObject(0);
	assertEqual(std::string("greet"), pGreet->getValue<std::string>("name"));
	assertEqual(std::string("Greeting"), pGreet->getValue<std::string>("title"));
	Array::Ptr pArguments = pGreet->getArray("arguments");
	assertEqual(2u, static_cast<unsigned>(pArguments->size()));
	assertTrue(pArguments->getObject(0)->getValue<bool>("required"));
	assertTrue(!pArguments->getObject(1)->getValue<bool>("required"));
}


void FeaturesTest::testPromptGet()
{
	Server server;
	buildServer(server);
	Object::Ptr pArgs = new Object;
	pArgs->set("who", "world");
	Object::Ptr pResult = result(getPrompt(server, 1, "greet", pArgs));
	assertEqual(std::string("complete"), pResult->getValue<std::string>("resultType"));
	assertEqual(std::string("Greets someone."), pResult->getValue<std::string>("description"));
	assertTrue(!pResult->has("ttlMs"));
	Object::Ptr pMessage = pResult->getArray("messages")->getObject(0);
	assertEqual(std::string("user"), pMessage->getValue<std::string>("role"));
	assertEqual(std::string("Hello world plainly"), pMessage->getObject("content")->getValue<std::string>("text"));
}


void FeaturesTest::testPromptArgumentErrors()
{
	Server server;
	buildServer(server);

	// The required argument is missing.
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(getPrompt(server, 1, "greet", nullptr)));
	Object::Ptr pArgs = new Object;
	pArgs->set("mood", "warmly");
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(getPrompt(server, 2, "greet", pArgs)));

	// Argument values are strings.
	pArgs = new Object;
	pArgs->set("who", 42);
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(getPrompt(server, 3, "greet", pArgs)));

	// Unknown prompt, and a name that is not a string.
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(getPrompt(server, 4, "nope", nullptr)));
	Object::Ptr pParams = new Object;
	pParams->set("name", true);
	assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(server.handle(request(Id(5), "prompts/get", pParams))));
}


void FeaturesTest::testCursorRefused()
{
	// Every list comes in one page, so no cursor is valid, the empty one included.
	Server server;
	buildServer(server);
	const char* methods[] = {"tools/list", "resources/list", "resources/templates/list", "prompts/list"};
	int id = 1;
	for (const char* method: methods)
	{
		Object::Ptr pParams = new Object;
		pParams->set("cursor", "");
		assertEqual(code(ErrorCode::INVALID_PARAMS), errorCode(server.handle(request(Id(id++), method, pParams))));
	}
}


void FeaturesTest::testRegistrationRefused()
{
	Server server;
	Resource resource;
	resource.name = "no uri";
	try
	{
		server.registerResource(resource, [](const std::string&, const RequestContext&) { return ResourceContents(); });
		fail("a resource without a URI must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
	resource.uri = "test://x";
	try
	{
		server.registerResource(resource, Poco::AI::MCP::ResourceReader());
		fail("a resource without a reader must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
	Prompt prompt;
	prompt.name = "p";
	try
	{
		server.registerPrompt(prompt, Poco::AI::MCP::PromptRenderer());
		fail("a prompt without a renderer must be refused");
	}
	catch (const Poco::InvalidArgumentException&)
	{
	}
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

	CppUnit_addTest(pSuite, FeaturesTest, testResourcesList);
	CppUnit_addTest(pSuite, FeaturesTest, testResourceReadText);
	CppUnit_addTest(pSuite, FeaturesTest, testResourceReadBlob);
	CppUnit_addTest(pSuite, FeaturesTest, testResourceNotFound);
	CppUnit_addTest(pSuite, FeaturesTest, testResourceReaderThrows);
	CppUnit_addTest(pSuite, FeaturesTest, testResourceTemplatesEmpty);
	CppUnit_addTest(pSuite, FeaturesTest, testPromptsList);
	CppUnit_addTest(pSuite, FeaturesTest, testPromptGet);
	CppUnit_addTest(pSuite, FeaturesTest, testPromptArgumentErrors);
	CppUnit_addTest(pSuite, FeaturesTest, testCursorRefused);
	CppUnit_addTest(pSuite, FeaturesTest, testRegistrationRefused);

	return pSuite;
}
