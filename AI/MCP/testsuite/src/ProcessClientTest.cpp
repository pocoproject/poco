//
// ProcessClientTest.cpp
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ProcessClientTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/AI/MCP/ProcessClient.h"
#include "Poco/AI/MCP/MCPException.h"
#include "Poco/JSON/Object.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Process.h"
#include "Poco/Exception.h"
#include <string>
#include <vector>


using Poco::AI::MCP::ProcessClient;
using Poco::AI::MCP::ToolResult;
using Poco::JSON::Object;


namespace
{
	std::string text(const ToolResult& result)
	{
		return result.content()->getObject(0)->getValue<std::string>("text");
	}


	Object::Ptr args(const std::string& key, const Poco::Dynamic::Var& value)
	{
		Object::Ptr pArgs = new Object;
		pArgs->set(key, value);
		return pArgs;
	}
}


ProcessClientTest::ProcessClientTest(const std::string& name):
	CppUnit::TestCase(name)
{
}


ProcessClientTest::~ProcessClientTest()
{
}


std::string ProcessClientTest::serverPath()
{
	// The server program is built next to the test runner.
	std::string name("AIMCPTestServer");
#if defined(_DEBUG)
	name += "d";
#endif
#if defined(POCO_OS_FAMILY_WINDOWS)
	name += ".exe";
#endif
	Poco::Path path(Poco::Path::self());
	path.setFileName(name);
	return path.toString();
}


void ProcessClientTest::testRoundTrip()
{
	ProcessClient client(serverPath(), std::vector<std::string>());
	assertEqual(std::string("test-server"), client.discover().serverInfo.name);
	assertEqual(4u, static_cast<unsigned>(client.listTools().size()));
	assertEqual(std::string("over stdio"), text(client.callTool("echo", args("message", "over stdio"))));
	assertEqual(0, client.close());
}


void ProcessClientTest::testTimeout()
{
	ProcessClient client(serverPath(), std::vector<std::string>());
	client.setTimeout(Poco::Timespan(0, 300000));
	try
	{
		(void) client.callTool("sleep", args("ms", 1500));
		fail("a reply later than the timeout must throw");
	}
	catch (const Poco::TimeoutException&)
	{
	}
	// The next call gets its own reply, not the late one.
	client.setTimeout(Poco::Timespan(30, 0));
	assertEqual(std::string("after the timeout"), text(client.callTool("echo", args("message", "after the timeout"))));
}


void ProcessClientTest::testServerEndsMidRequest()
{
	ProcessClient client(serverPath(), std::vector<std::string>());
	try
	{
		(void) client.callTool("exit", args("code", 7));
		fail("a server that ends before the reply must throw");
	}
	catch (const Poco::IOException&)
	{
	}
	assertEqual(7, client.close());
	try
	{
		(void) client.callTool("echo");
		fail("a closed client must throw");
	}
	catch (const Poco::IOException&)
	{
	}
}


void ProcessClientTest::testNoiseOnStandardOutput()
{
	ProcessClient client(serverPath(), std::vector<std::string>());
	assertEqual(std::string("after noise"), text(client.callTool("noise")));
	assertEqual(std::string("still fine"), text(client.callTool("echo", args("message", "still fine"))));
}


void ProcessClientTest::testCloseEndsServer()
{
	ProcessClient client(serverPath(), std::vector<std::string>());
	const Poco::Process::PID pid = client.id();
	assertTrue(Poco::Process::isRunning(pid));
	assertEqual(0, client.close());
	assertTrue(!Poco::Process::isRunning(pid));
	// A second close returns the same exit code.
	assertEqual(0, client.close());
}


void ProcessClientTest::testStubbornServerIsKilled()
{
	std::vector<std::string> arguments;
	arguments.push_back("--stubborn");
	ProcessClient client(serverPath(), arguments);
	client.setGracePeriod(Poco::Timespan(0, 200000));
	assertEqual(std::string("x"), text(client.callTool("echo", args("message", "x"))));
	const Poco::Process::PID pid = client.id();
	(void) client.close();
	assertTrue(!Poco::Process::isRunning(pid));
}


void ProcessClientTest::setUp()
{
	if (!Poco::File(serverPath()).exists())
	{
		fail("The test server program is missing: " + serverPath());
	}
}


void ProcessClientTest::tearDown()
{
}


CppUnit::Test* ProcessClientTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ProcessClientTest");

	CppUnit_addTest(pSuite, ProcessClientTest, testRoundTrip);
	CppUnit_addTest(pSuite, ProcessClientTest, testTimeout);
	CppUnit_addTest(pSuite, ProcessClientTest, testServerEndsMidRequest);
	CppUnit_addTest(pSuite, ProcessClientTest, testNoiseOnStandardOutput);
	CppUnit_addTest(pSuite, ProcessClientTest, testCloseEndsServer);
	CppUnit_addTest(pSuite, ProcessClientTest, testStubbornServerIsKilled);

	return pSuite;
}
