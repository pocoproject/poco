//
// ProcessTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ProcessTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Process.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/Path.h"
#include "Poco/Format.h"
#include "Poco/Environment.h"
#include "Poco/ProcessOptions.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include <atomic>
#include <csignal>
#include <iostream>


using namespace std::string_literals;
using Poco::Process;
using Poco::ProcessHandle;
using Poco::Pipe;
using Poco::Path;
using Poco::PipeInputStream;
using Poco::PipeOutputStream;
using Poco::Thread;


ProcessTest::ProcessTest(const std::string& name): CppUnit::TestCase(name)
{
}


ProcessTest::~ProcessTest()
{
}


void ProcessTest::testEscapeArgs()
{
#if defined(_WIN32)
	assertTrue (Poco::ProcessImpl::mustEscapeArg("a b"));
	assertFalse (Poco::ProcessImpl::mustEscapeArg("abc"));
	assertFalse (Poco::ProcessImpl::mustEscapeArg("\"a b \""));
	assertFalse (Poco::ProcessImpl::mustEscapeArg("\"abc\""));
	assertTrue (Poco::ProcessImpl::mustEscapeArg("\"a b "));
	assertFalse (Poco::ProcessImpl::mustEscapeArg("/arg=\"a b c\""));

	assertEquals ("abc"s, Poco::ProcessImpl::escapeArg("abc"));
	assertEquals ("\"a b c\""s, Poco::ProcessImpl::escapeArg("a b c"));
	assertEquals ("\"a b \\\" c\""s, Poco::ProcessImpl::escapeArg("a b \" c"));
	assertEquals ("/arg=\"a b c\""s, Poco::ProcessImpl::escapeArg("/arg=\"a b c\""));
#endif
}


void ProcessTest::testLaunch()
{
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

#if defined(POCO_OS_FAMILY_UNIX)
	cmd += name;
#else
	cmd = name;
#endif

	std::vector<std::string> args;
	args.push_back("arg1");
	args.push_back("arg2");
	args.push_back("arg3");
	ProcessHandle ph = Process::launch(cmd, args);
	int rc = ph.wait();
	assertTrue (rc == 3);
}


void ProcessTest::testLaunchRedirectIn()
{
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

#if defined(POCO_OS_FAMILY_UNIX)
	cmd += name;
#else
	cmd = name;
#endif

	std::vector<std::string> args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(cmd, args, &inPipe, nullptr, nullptr);
	PipeOutputStream ostr(inPipe);
	ostr << std::string(100, 'x');
	ostr.close();
	int rc = ph.wait();
	assertTrue (rc == 100);
}


void ProcessTest::testLaunchRedirectOut()
{
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

#if defined(POCO_OS_FAMILY_UNIX)
	cmd += name;
#else
	cmd = name;
#endif

	std::vector<std::string> args;
	args.push_back("-hello");
	Pipe outPipe;
	ProcessHandle ph = Process::launch(cmd, args, nullptr, &outPipe, nullptr);
	PipeInputStream istr(outPipe);
	std::string s;
	int c = istr.get();
	while (c != -1) { s += (char) c; c = istr.get(); }
	assertTrue (s == "Hello, world!");
	int rc = ph.wait();
	assertTrue (rc == 1);
}


void ProcessTest::testLaunchEnv()
{
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

#if defined(POCO_OS_FAMILY_UNIX)
	cmd += name;
#else
	cmd = name;
#endif

	std::vector<std::string> args;
	args.push_back("-env");
	Pipe outPipe;
	Process::Env env;
	env["TESTENV"] = "test";
#if defined(_WIN32)
	env["PATH"] = Poco::Environment::get("PATH"); // required for VS
#endif
	ProcessHandle ph = Process::launch(cmd, args, nullptr, &outPipe, nullptr, env);
	PipeInputStream istr(outPipe);
	std::string s;
	int c = istr.get();
	while (c != -1) { s += (char) c; c = istr.get(); }
	assertTrue (s == "test");
	int rc = ph.wait();
	assertTrue (rc == 0);
}


void ProcessTest::testLaunchArgs()
{
#if defined (_WIN32)
	std::string name("TestApp");
#if defined(_DEBUG)
	name += 'd';
#endif
	std::string cmd = name;

	std::vector<std::string> args;
	args.push_back("-echo-args");
	args.push_back("simple");
	args.push_back("with space");
	args.push_back("with\ttab");
	args.push_back("with\vverticaltab");
	// can't test newline here because TestApp -echo-args uses newline to separate the echoed args
	//args.push_back("with\nnewline");
	args.push_back("with \" quotes");
	args.push_back("ends with \"quotes\"");
	args.push_back("\"starts\" with quotes");
	args.push_back("\"");
	args.push_back("\\");
	args.push_back("c:\\program files\\ends with backslash\\");
	args.push_back("\"already quoted \\\" \\\\\"");
	Pipe outPipe;
	ProcessHandle ph = Process::launch(cmd, args, 0, &outPipe, 0);
	PipeInputStream istr(outPipe);
	std::string receivedArg;
	int c = istr.get();
	int argNumber = 1;
	while (c != -1)
	{
		if ('\n' == c)
		{
			assertTrue (argNumber < args.size());
			std::string expectedArg = args[argNumber];
			if (expectedArg.npos != expectedArg.find("already quoted")) {
				expectedArg = "already quoted \" \\";
			}
			assertTrue (receivedArg == expectedArg);
			++argNumber;
			receivedArg = "";
		}
		else if ('\r' != c)
		{
			receivedArg += (char)c;
		}
		c = istr.get();
	}
	assertTrue (argNumber == args.size());
	int rc = ph.wait();
	assertTrue (rc == args.size());
#endif // defined (_WIN32)
}


void ProcessTest::testLaunchInvalidCommand()
{
	std::string name("InvalidCmd");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

#if defined(POCO_OS_FAMILY_UNIX)
	cmd += name;
#else
	cmd = name;
#endif

	std::vector<std::string> args;
	args.push_back("arg1");
	args.push_back("arg2");
	args.push_back("arg3");
#if defined(POCO_OS_FAMILY_UNIX)
	ProcessHandle ph = Process::launch(cmd, args);
	int rc = ph.wait();
	assertTrue (rc == 72);
#elif defined(POCO_OS_FAMILY_WINDOWS)
	try
	{
		ProcessHandle ph = Process::launch(cmd, args);
		int rc = ph.wait();
		fail("must fail");
	}
	catch (...){}
#endif
}


void ProcessTest::testIsRunning()
{
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

#if defined(POCO_OS_FAMILY_UNIX)
	cmd += name;
#else
	cmd = name;
#endif

	std::vector<std::string> args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(cmd, args, &inPipe, nullptr, nullptr);
	Process::PID id = ph.id();
	assertTrue (Process::isRunning(ph));
	assertTrue (Process::isRunning(id));
	PipeOutputStream ostr(inPipe);
	ostr << std::string(100, 'x');
	ostr.close();
	[[maybe_unused]] int rc = ph.wait();
	assertTrue (!Process::isRunning(ph));
	assertTrue (!Process::isRunning(id));
}


void ProcessTest::testLaunchCloseHandles()
{
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

#if defined(POCO_OS_FAMILY_UNIX)
	cmd += name;
#else
	cmd = name;
#endif

	// Test PROCESS_CLOSE_STDIN: verify the flag doesn't crash parent or child.
	{
		std::vector<std::string> args;
		args.push_back("-hello");
		ProcessHandle ph = Process::launch(cmd, args, Poco::PROCESS_CLOSE_STDIN);
		int rc = ph.wait();
		assertTrue (rc == 1);
	}

	// Test PROCESS_CLOSE_STDOUT: child runs in -hello mode but with its stdout closed.
	// Critically, the parent's stdout must still work afterward.
	{
		std::vector<std::string> args;
		args.push_back("-hello");
		ProcessHandle ph = Process::launch(cmd, args, Poco::PROCESS_CLOSE_STDOUT);
		int rc = ph.wait();
		assertTrue (rc == 1);
		std::cout << "parent stdout ok" << std::flush;
		assertTrue (std::cout.good());
	}

	// Test PROCESS_CLOSE_STDERR: the parent's stderr must still work afterward.
	{
		std::vector<std::string> args;
		args.push_back("arg1");
		ProcessHandle ph = Process::launch(cmd, args, Poco::PROCESS_CLOSE_STDERR);
		int rc = ph.wait();
		assertTrue (rc == 1);
		std::cerr << "parent stderr ok" << std::flush;
		assertTrue (std::cerr.good());
	}

	// Test all flags combined.
	{
		std::vector<std::string> args;
		args.push_back("-hello");
		ProcessHandle ph = Process::launch(cmd, args,
			Poco::PROCESS_CLOSE_STDIN | Poco::PROCESS_CLOSE_STDOUT | Poco::PROCESS_CLOSE_STDERR);
		int rc = ph.wait();
		assertTrue (rc == 1);
		std::cout << "parent stdout ok" << std::flush;
		assertTrue (std::cout.good());
		std::cerr << "parent stderr ok" << std::flush;
		assertTrue (std::cerr.good());
	}
}


void ProcessTest::testIsRunningAllowsForTermination()
{
#if !defined(_WIN32_WCE)
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

#if defined(POCO_OS_FAMILY_UNIX)
	cmd += name;
#else
	cmd = name;
#endif

	// Launch with pipes to reproduce the original defunct process scenario (issue #1097).
	// When a process launched with pipes exits, it becomes defunct until reaped.
	std::vector<std::string> args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(cmd, args, &inPipe, nullptr, nullptr);
	PipeOutputStream ostr(inPipe);
	ostr << std::string(10, 'x');
	ostr.close();
	Poco::Timestamp waitStart;
	while (Process::isRunning(ph))
	{
		if (waitStart.isElapsed(30 * Poco::Timestamp::resolution())) fail("Process did not terminate within 30 seconds");
		Thread::sleep(100);
	}
	// Process should be reaped by now; wait must not hang
	int rc = ph.wait();
	assertTrue (rc == 10);
#endif // !defined(_WIN32_WCE)
}


void ProcessTest::testIsRunningByPidAllowsForTermination()
{
#if defined(POCO_OS_FAMILY_UNIX)
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

	cmd += name;

	// Test the PID-only isRunning overload with a child process
	std::vector<std::string> args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(cmd, args, &inPipe, nullptr, nullptr);
	Process::PID pid = ph.id();
	PipeOutputStream ostr(inPipe);
	ostr << std::string(10, 'x');
	ostr.close();
	Poco::Timestamp waitStart;
	while (Process::isRunning(pid))
	{
		if (waitStart.isElapsed(30 * Poco::Timestamp::resolution())) fail("Process did not terminate within 30 seconds");
		Thread::sleep(100);
	}
#endif // defined(POCO_OS_FAMILY_UNIX)
}


void ProcessTest::testWaitAfterIsRunning()
{
#if !defined(_WIN32_WCE)
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

#if defined(POCO_OS_FAMILY_UNIX)
	cmd += name;
#else
	cmd = name;
#endif

	// Verify that calling wait() after isRunning() returns false works correctly.
	// isRunning() reaps the process internally via waitpid; wait() must still succeed.
	std::vector<std::string> args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(cmd, args, &inPipe, nullptr, nullptr);
	PipeOutputStream ostr(inPipe);
	ostr << std::string(42, 'x');
	ostr.close();
	Poco::Timestamp waitStart;
	while (Process::isRunning(ph))
	{
		if (waitStart.isElapsed(30 * Poco::Timestamp::resolution())) fail("Process did not terminate within 30 seconds");
		Thread::sleep(100);
	}
	int rc = ph.wait();
	assertTrue (rc == 42);
#endif // !defined(_WIN32_WCE)
}


void ProcessTest::testConcurrentWaitAndIsRunning()
{
#if defined(POCO_OS_FAMILY_UNIX)
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

	cmd += name;

	// Test thread safety: one thread polls isRunning while another calls wait.
	std::vector<std::string> args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(cmd, args, &inPipe, nullptr, nullptr);
	PipeOutputStream ostr(inPipe);

	std::atomic<int> waitResult{-1};
	std::atomic<bool> waitDone{false};

	Thread waiter;
	waiter.startFunc([&ph, &waitResult, &waitDone]()
	{
		waitResult = ph.wait();
		waitDone = true;
	});

	// Let the waiter thread start, then close the pipe to let the process exit
	Thread::sleep(100);
	ostr << std::string(7, 'x');
	ostr.close();

	// Poll isRunning concurrently with the waiter thread
	Poco::Timestamp waitStart;
	while (Process::isRunning(ph))
	{
		if (waitStart.isElapsed(30 * Poco::Timestamp::resolution())) fail("Process did not terminate within 30 seconds");
		Thread::sleep(50);
	}

	waiter.join();
	assertTrue (waitDone);
	assertTrue (waitResult == 7);
#endif // defined(POCO_OS_FAMILY_UNIX)
}


void ProcessTest::testSignalExitCode()
{
#if defined(POCO_OS_FAMILY_UNIX)
	std::string name("TestApp");
	std::string cmd;
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

	cmd += name;

	std::vector<std::string> args;
	args.push_back("-raise-int");
	ProcessHandle ph = Process::launch(cmd, args, nullptr, nullptr, nullptr);
	int rc = ph.wait();
	assertEqual (256 + SIGINT, rc);
#endif // defined(POCO_OS_FAMILY_UNIX)
}


void ProcessTest::testIsRunningByPidThenWaitOnHandle()
{
#if defined(POCO_OS_FAMILY_UNIX)
	std::string cmd = ProcessTest::getFullName("TestApp");

	// Verify that polling isRunning(pid) does NOT prevent a subsequent
	// handle.wait() from succeeding. The PID-based overload must not
	// reap the child (uses waitid with WNOWAIT).
	std::vector<std::string> args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(cmd, args, &inPipe, nullptr, nullptr);
	Process::PID pid = ph.id();
	PipeOutputStream ostr(inPipe);
	ostr << std::string(13, 'x');
	ostr.close();
	Poco::Timestamp waitStart;
	while (Process::isRunning(pid))
	{
		if (waitStart.isElapsed(30 * Poco::Timestamp::resolution())) fail("Process did not terminate within 30 seconds");
		Thread::sleep(100);
	}
	// The critical part: wait() must not hang or throw after PID-based isRunning() returned false
	int rc = ph.wait();
	assertTrue (rc == 13);
#endif // defined(POCO_OS_FAMILY_UNIX)
}


void ProcessTest::testIsRunningAfterWait()
{
	std::string cmd = ProcessTest::getFullName("TestApp");

	// Verify that isRunning() returns false after wait() has completed.
	std::vector<std::string> args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(cmd, args, &inPipe, nullptr, nullptr);
	Process::PID pid = ph.id();
	PipeOutputStream ostr(inPipe);
	ostr << std::string(5, 'x');
	ostr.close();
	int rc = ph.wait();
	assertTrue (rc == 5);
	assertTrue (!Process::isRunning(ph));
#if defined(POCO_OS_FAMILY_UNIX)
	assertTrue (!Process::isRunning(pid));
#endif
}


void ProcessTest::setUp()
{
}


void ProcessTest::tearDown()
{
}


std::string ProcessTest::getFullName(const std::string& name)
{
	std::string fullName(name);
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	fullName += "d";
#endif
	return fullName;
}


CppUnit::Test* ProcessTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ProcessTest");

	CppUnit_addTest(pSuite, ProcessTest, testEscapeArgs);
	CppUnit_addTest(pSuite, ProcessTest, testLaunch);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchRedirectIn);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchRedirectOut);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchEnv);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchArgs);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchInvalidCommand);
	CppUnit_addTest(pSuite, ProcessTest, testIsRunning);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchCloseHandles);
	CppUnit_addTest(pSuite, ProcessTest, testIsRunningAllowsForTermination);
	CppUnit_addTest(pSuite, ProcessTest, testIsRunningByPidAllowsForTermination);
	CppUnit_addTest(pSuite, ProcessTest, testWaitAfterIsRunning);
	CppUnit_addTest(pSuite, ProcessTest, testConcurrentWaitAndIsRunning);
	CppUnit_addTest(pSuite, ProcessTest, testSignalExitCode);
	CppUnit_addTest(pSuite, ProcessTest, testIsRunningByPidThenWaitOnHandle);
	CppUnit_addTest(pSuite, ProcessTest, testIsRunningAfterWait);

	return pSuite;
}
