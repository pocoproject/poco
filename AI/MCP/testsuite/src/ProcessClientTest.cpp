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
#include "Poco/JSON/Parser.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/NumberParser.h"
#include "Poco/Process.h"
#include "Poco/Environment.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include <fstream>
#include <string>
#include <vector>
#if defined(POCO_OS_FAMILY_UNIX)
#include <pthread.h>
#include <signal.h>
#endif


using Poco::AI::MCP::ProcessClient;
using Poco::AI::MCP::ToolResult;
using Poco::JSON::Object;


namespace
{
#if defined(POCO_OS_FAMILY_UNIX)
	class PipeSignalLetThrough
		/// Lets SIGPIPE through to the calling thread while it lives.
	{
	public:
		PipeSignalLetThrough()
		{
			sigset_t pipeSignal;
			sigemptyset(&pipeSignal);
			sigaddset(&pipeSignal, SIGPIPE);
			pthread_sigmask(SIG_UNBLOCK, &pipeSignal, &_saved);
		}

		~PipeSignalLetThrough()
		{
			pthread_sigmask(SIG_SETMASK, &_saved, nullptr);
		}

	private:
		sigset_t _saved;
	};
#endif


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


	std::vector<std::string> mode(const std::string& name)
	{
		return std::vector<std::string>(1, name);
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
	assertEqual(9u, static_cast<unsigned>(client.listTools().size()));
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


void ProcessClientTest::testWriteToEndedServer()
{
#if defined(POCO_OS_FAMILY_UNIX)
	// The threads of the client take over the signal mask of the thread that
	// makes it, which lets SIGPIPE through unless Foundation made it.
	PipeSignalLetThrough letThrough;
#endif
	ProcessClient client(serverPath(), std::vector<std::string>());
	try
	{
		(void) client.callTool("exit", args("code", 7));
		fail("a server that ends before the reply must throw");
	}
	catch (const Poco::IOException&)
	{
	}
	// The write to the server that has ended fails, and this process lives
	// on. Until the server has closed its standard input as well, a line may
	// still go into the pipe, and the call fails as the server has ended.
	bool refused = false;
	const Poco::Timestamp writing;
	while (!refused && writing.elapsed() < 10 * Poco::Timestamp::resolution())
	{
		try
		{
			(void) client.callTool("echo");
			fail("a call to a server that has ended must throw");
		}
		catch (const Poco::IOException& exc)
		{
			refused = Poco::startsWith(exc.message(), std::string("Cannot write to the MCP server process"));
		}
	}
	assertTrue(refused);
	assertEqual(7, client.close());
}


void ProcessClientTest::testWriteTimeoutCancels()
{
	// A request that times out while it is being written reaches the server
	// whole once it reads again, so the client cancels it.
	Poco::TemporaryFile inputLog;
	Poco::Process::Env env;
	env["MCP_TEST_INPUT_LOG"] = inputLog.path();
	ProcessClient client(serverPath(), std::vector<std::string>(), std::string(), env);
	client.setGracePeriod(Poco::Timespan(20, 0));
	// While the server sleeps it reads nothing.
	client.setTimeout(Poco::Timespan(0, 100000));
	try
	{
		(void) client.callTool("sleep", args("ms", 2000));
		fail("a call without a reply in time must time out");
	}
	catch (const Poco::TimeoutException&)
	{
	}
	// More than a pipe holds, so the line is still being written when the
	// call times out.
	client.setTimeout(Poco::Timespan(0, 200000));
	try
	{
		(void) client.callTool("echo", args("message", std::string(1024 * 1024, 'x')));
		fail("a call that cannot be written in time must time out");
	}
	catch (const Poco::TimeoutException&)
	{
	}
	// The next call goes out after the cancel, and gets its own reply once
	// the server has read the rest of the request.
	client.setTimeout(Poco::Timespan(30, 0));
	assertEqual(std::string("after"), text(client.callTool("echo", args("message", "after"))));
	assertEqual(0, client.close());

	// The input of the server: the echo request, and a cancel of its id.
	std::ifstream in(inputLog.path(), std::ios::binary);
	Poco::Dynamic::Var echoId;
	bool cancelled = false;
	std::string line;
	while (std::getline(in, line))
	{
		Poco::JSON::Parser parser;
		const Object::Ptr pMessage = parser.parse(line).extract<Object::Ptr>();
		const std::string method = pMessage->optValue<std::string>("method", "");
		const Object::Ptr pParams = pMessage->getObject("params");
		if (method == "tools/call" && pParams->optValue<std::string>("name", "") == "echo") echoId = pMessage->get("id");
		if (method == "notifications/cancelled" && !echoId.isEmpty() && pParams->get("requestId") == echoId) cancelled = true;
	}
	assertTrue(!echoId.isEmpty());
	assertTrue(cancelled);
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


void ProcessClientTest::testTerminationRequest()
{
	// A server that does not end at the end of its input ends on the
	// termination request, with its own exit code.
	ProcessClient client(serverPath(), mode("--interruptible"));
	client.setGracePeriod(Poco::Timespan(0, 300000));
	assertEqual(std::string("x"), text(client.callTool("echo", args("message", "x"))));
	const Poco::Process::PID pid = client.id();
	assertEqual(42, client.close());
	assertTrue(!Poco::Process::isRunning(pid));
}


void ProcessClientTest::testEnvironmentAndDirectory()
{
	Poco::TemporaryFile directory;
	directory.createDirectories();
	Poco::Process::Env env;
	env["POCO_AI_MCP_TEST"] = "forty-two";
	ProcessClient client(serverPath(), std::vector<std::string>(), directory.path(), env);
	assertEqual(std::string("forty-two"), text(client.callTool("getenv", args("name", "POCO_AI_MCP_TEST"))));
	// The directory may be reported through links; its own name is enough.
	const std::string current = text(client.callTool("cwd"));
	assertTrue(current.find(Poco::Path(directory.path()).getFileName()) != std::string::npos);
	assertEqual(0, client.close());
}


void ProcessClientTest::testClearedEnvironment()
{
	// A variable of this process does not reach a server started with a
	// cleared environment; a given one does.
	Poco::Environment::set("POCO_AI_MCP_INHERITED", "inherited");
	Poco::Process::Env env;
	// What the server needs to load the libraries.
	for (const std::string variable: {"PATH", "LD_LIBRARY_PATH", "DYLD_LIBRARY_PATH", "SystemRoot"})
	{
		if (Poco::Environment::has(variable)) env[variable] = Poco::Environment::get(variable);
	}
	env["POCO_AI_MCP_TEST"] = "forty-two";
	ProcessClient client(serverPath(), std::vector<std::string>(), std::string(), env, ProcessClient::Environment::CLEARED);
	assertEqual(std::string("forty-two"), text(client.callTool("getenv", args("name", "POCO_AI_MCP_TEST"))));
	assertEqual(std::string(), text(client.callTool("getenv", args("name", "POCO_AI_MCP_INHERITED"))));
	assertEqual(0, client.close());
}


void ProcessClientTest::testOversizedLine()
{
	// A line over the limit fails the request it comes during; the client
	// stays usable.
	ProcessClient client(serverPath(), std::vector<std::string>(), std::string(), Poco::Process::Env(), ProcessClient::Environment::INHERITED, 1024);
	try
	{
		(void) client.callTool("big", args("bytes", 4096));
		fail("a reply over the size limit must throw");
	}
	catch (const Poco::ProtocolException&)
	{
	}
	assertEqual(std::string("small"), text(client.callTool("echo", args("message", "small"))));
}


void ProcessClientTest::testFloodBeforeTheReply()
{
	// Notifications before the reply are passed over, however many come.
	ProcessClient client(serverPath(), std::vector<std::string>());
	assertEqual(std::string("flooded"), text(client.callTool("flood", args("count", 5000))));
	assertEqual(std::string("then"), text(client.callTool("echo", args("message", "then"))));
}


void ProcessClientTest::testServerStopsReading()
{
	// While a tool runs the server reads nothing: a request too large for
	// the pipe cannot be written, times out, and does not hold up close().
	ProcessClient client(serverPath(), std::vector<std::string>());
	client.setTimeout(Poco::Timespan(0, 500000));
	client.setGracePeriod(Poco::Timespan(0, 300000));
	try
	{
		(void) client.callTool("sleep", args("ms", 60000));
		fail("a tool that runs past the timeout must throw");
	}
	catch (const Poco::TimeoutException&)
	{
	}
	const Poco::Timestamp writing;
	try
	{
		(void) client.callTool("echo", args("message", std::string(1024 * 1024, 'x')));
		fail("a request the server does not read must time out");
	}
	catch (const Poco::TimeoutException&)
	{
	}
	assertTrue(writing.elapsed() < 10 * Poco::Timestamp::resolution());
	const Poco::Process::PID pid = client.id();
	const Poco::Timestamp closing;
	(void) client.close();
	assertTrue(closing.elapsed() < 10 * Poco::Timestamp::resolution());
	assertTrue(!Poco::Process::isRunning(pid));
}


void ProcessClientTest::testProcessKeepsOutputOpen()
{
	// The server starts a process that keeps its standard output open and
	// does not end on the termination request. On POSIX platforms it ends
	// with the server's group; where it does not, close() still returns.
	ProcessClient client(serverPath(), mode("--stubborn"));
	client.setGracePeriod(Poco::Timespan(0, 200000));
	const Poco::Process::PID lingering = static_cast<Poco::Process::PID>(Poco::NumberParser::parse64(text(client.callTool("spawn"))));
	const Poco::Timestamp closing;
	(void) client.close();
	assertTrue(closing.elapsed() < 10 * Poco::Timestamp::resolution());
#if defined(POCO_OS_FAMILY_UNIX)
	// Killed with the group, it is reaped by whichever process adopted it.
	const Poco::Timestamp waiting;
	while (Poco::Process::isRunning(lingering) && waiting.elapsed() < 10 * Poco::Timestamp::resolution())
	{
		Poco::Thread::sleep(20);
	}
	assertTrue(!Poco::Process::isRunning(lingering));
#else
	if (Poco::Process::isRunning(lingering)) Poco::Process::kill(lingering);
#endif
}


void ProcessClientTest::testReaderOutlivesClient()
{
	// A process of a group of its own keeps the standard output of the server
	// open past the server: close() returns, and the reader is left to end
	// when that process does.
	ProcessClient client(serverPath(), mode("--stubborn"));
	client.setGracePeriod(Poco::Timespan(0, 200000));
	const Poco::Process::PID lingering = static_cast<Poco::Process::PID>(Poco::NumberParser::parse64(text(client.callTool("spawn", args("group", "own")))));
	const Poco::Timestamp closing;
	(void) client.close();
	assertTrue(closing.elapsed() < 10 * Poco::Timestamp::resolution());
	assertTrue(Poco::Process::isRunning(lingering));
	Poco::Process::kill(lingering);
}


void ProcessClientTest::testCommandThatCannotRun()
{
	const std::string missing = Poco::Path(serverPath()).setFileName("no-such-mcp-server").toString();
#if defined(POCO_OS_FAMILY_UNIX)
	// The process is made, and ends as it starts.
	ProcessClient client(missing, std::vector<std::string>());
	try
	{
		(void) client.callTool("echo");
		fail("a server that could not start must fail the request");
	}
	catch (const Poco::IOException&)
	{
	}
	assertEqual(72, client.close());
#else
	try
	{
		ProcessClient client(missing, std::vector<std::string>());
		fail("a command that cannot run must throw");
	}
	catch (const Poco::SystemException&)
	{
	}
#endif
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
	CppUnit_addTest(pSuite, ProcessClientTest, testWriteToEndedServer);
	CppUnit_addTest(pSuite, ProcessClientTest, testWriteTimeoutCancels);
	CppUnit_addTest(pSuite, ProcessClientTest, testReaderOutlivesClient);
	CppUnit_addTest(pSuite, ProcessClientTest, testNoiseOnStandardOutput);
	CppUnit_addTest(pSuite, ProcessClientTest, testCloseEndsServer);
	CppUnit_addTest(pSuite, ProcessClientTest, testStubbornServerIsKilled);
	CppUnit_addTest(pSuite, ProcessClientTest, testTerminationRequest);
	CppUnit_addTest(pSuite, ProcessClientTest, testEnvironmentAndDirectory);
	CppUnit_addTest(pSuite, ProcessClientTest, testClearedEnvironment);
	CppUnit_addTest(pSuite, ProcessClientTest, testOversizedLine);
	CppUnit_addTest(pSuite, ProcessClientTest, testFloodBeforeTheReply);
	CppUnit_addTest(pSuite, ProcessClientTest, testServerStopsReading);
	CppUnit_addTest(pSuite, ProcessClientTest, testProcessKeepsOutputOpen);
	CppUnit_addTest(pSuite, ProcessClientTest, testCommandThatCannotRun);

	return pSuite;
}
