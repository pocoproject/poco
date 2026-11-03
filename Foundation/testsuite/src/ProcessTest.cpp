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
#include "Poco/Exception.h"
#include "Poco/Bugcheck.h"
#include "Poco/ProcessOptions.h"
#include "Poco/File.h"
#include "Poco/Stopwatch.h"
#include "Poco/StreamCopier.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include <atomic>
#include <csignal>
#include <cstdlib>
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


void ProcessTest::testLaunchClearedEnvironment()
{
	std::string name("TestApp");
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	name += "d";
#endif

	// A variable of this process, which a child launched with a cleared
	// environment does not get.
	Poco::Environment::set("POCO_TEST_INHERITED", "inherited");
	Process::Env env;
	// What the child needs to be found and to load the libraries.
	for (const std::string variable: {"PATH", "LD_LIBRARY_PATH", "DYLD_LIBRARY_PATH", "SystemRoot"})
	{
		if (Poco::Environment::has(variable)) env[variable] = Poco::Environment::get(variable);
	}
	env["TESTENV"] = "given";

	const auto childValue = [&name, &env](const std::string& variable, int options)
		/// The value of variable in the environment of the child, or
		/// "(none)" when it has none.
	{
		Pipe outPipe;
		ProcessHandle ph = Process::launch(name, {"-getenv", variable}, nullptr, &outPipe, nullptr, env, options);
		PipeInputStream istr(outPipe);
		std::string value;
		int c = istr.get();
		while (c != -1) { value += static_cast<char>(c); c = istr.get(); }
		return ph.wait() == 0 ? value : std::string("(none)");
	};

	assertEqual(std::string("given"), childValue("TESTENV", Poco::PROCESS_CLEAR_ENVIRONMENT));
	assertEqual(std::string("(none)"), childValue("POCO_TEST_INHERITED", Poco::PROCESS_CLEAR_ENVIRONMENT));
	// Without the option, the child inherits the variables of this process,
	// except on QNX, where a child spawned with variables gets only those.
#if POCO_OS != POCO_OS_QNX
	assertEqual(std::string("inherited"), childValue("POCO_TEST_INHERITED", 0));
#endif

	// Without any variable given, the child has none.
#if defined(POCO_OS_FAMILY_WINDOWS)
	ProcessHandle bare = Process::launch(name, {"-getenv", "PATH"}, nullptr, nullptr, nullptr, Process::Env(), Poco::PROCESS_CLEAR_ENVIRONMENT);
	assertEqual(1, bare.wait());
#elif defined(POCO_OS_FAMILY_UNIX) && (POCO_OS != POCO_OS_QNX)
	// env is found through the default search path, and prints what it has.
	Pipe outPipe;
	ProcessHandle bare = Process::launch("env", Process::Args(), nullptr, &outPipe, nullptr, Process::Env(), Poco::PROCESS_CLEAR_ENVIRONMENT);
	PipeInputStream istr(outPipe);
	std::string output;
	int c = istr.get();
	while (c != -1) { output += static_cast<char>(c); c = istr.get(); }
	assertEqual(0, bare.wait());
	assertEqual(std::string(), output);
#endif
}


namespace
{
	std::string launchEnvApp(const std::string& command, const Process::Env& env, int& rc)
		/// Launches TestApp with the given environment variables and returns
		/// what it writes to standard output: the value of TESTENV.
	{
		Pipe outPipe;
		ProcessHandle ph = Process::launch(command, {"-env"}, nullptr, &outPipe, nullptr, env);
		PipeInputStream istr(outPipe);
		std::string s;
		Poco::StreamCopier::copyToString(istr, s);
		rc = ph.wait();
		return s;
	}

	class ScopedEnvironmentVariable
		/// Sets an environment variable of the test runner and restores its
		/// previous state when destroyed, so that later tests do not see it.
	{
	public:
		ScopedEnvironmentVariable() = delete;

		POCO_NODISCARD_CTOR ScopedEnvironmentVariable(const std::string& name, const std::string& value):
			_name(name),
			_wasSet(Poco::Environment::has(name)),
			_previousValue(Poco::Environment::get(name, ""))
		{
			Poco::Environment::set(name, value);
		}

		~ScopedEnvironmentVariable()
		{
			try
			{
				if (_wasSet)
				{
					Poco::Environment::set(_name, _previousValue);
				}
				else
				{
					// Poco::Environment cannot remove a variable.
#if defined(POCO_OS_FAMILY_WINDOWS)
					SetEnvironmentVariableA(_name.c_str(), nullptr);
#else
					unsetenv(_name.c_str());
#endif
				}
			}
			catch (...)
			{
				poco_unexpected();
			}
		}

		ScopedEnvironmentVariable(const ScopedEnvironmentVariable&) = delete;
		ScopedEnvironmentVariable(ScopedEnvironmentVariable&&) = delete;
		ScopedEnvironmentVariable& operator = (const ScopedEnvironmentVariable&) = delete;
		ScopedEnvironmentVariable& operator = (ScopedEnvironmentVariable&&) = delete;

	private:
		const std::string _name;
		const bool _wasSet;
		const std::string _previousValue;
	};
}


void ProcessTest::testLaunchEnvKeepsParentEnvironment()
{
	// The variables given to launch() are added to the environment of the
	// calling process, which the child keeps; on QNX, a child spawned with
	// variables gets only those.
#if POCO_OS != POCO_OS_QNX
	const ScopedEnvironmentVariable testEnv("TESTENV", "parent");
	Process::Env env;
	env["TESTENV_OTHER"] = "other";
	int rc = -1;
	assertEqual ("parent"s, launchEnvApp(getFullName("TestApp"), env, rc));
	assertEqual (0, rc);
#endif
}


void ProcessTest::testLaunchEnvReplacesParentVariable()
{
	// Windows compares the names of environment variables without regard to case.
#if defined(POCO_OS_FAMILY_WINDOWS)
	const std::string name("testenv");
#else
	const std::string name("TESTENV");
#endif
	const ScopedEnvironmentVariable testEnv("TESTENV", "parent");
	Process::Env env;
	env[name] = "child";
	int rc = -1;
	assertEqual ("child"s, launchEnvApp(getFullName("TestApp"), env, rc));
	assertEqual (0, rc);
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
	(void) ph.wait();
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


void ProcessTest::testLaunchClosedHandlesStayTaken()
{
#if defined(POCO_OS_FAMILY_UNIX)
	// A standard stream that is closed for the child keeps its descriptor
	// taken, so that nothing the child opens becomes its standard input,
	// output or error. TestApp returns which of the three it has: with all
	// of them, 1 + 2 + 4. The streams that are not closed are given pipes
	// here, so that the outcome does not depend on what this process has
	// for its own.
	std::string cmd("TestApp");
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
	cmd += "d";
#endif
	std::vector<std::string> args;
	args.push_back("-std-fds");
	const int all = 7;

	{
		Pipe outPipe;
		Pipe errPipe;
		ProcessHandle ph = Process::launch(cmd, args, nullptr, &outPipe, &errPipe, Poco::PROCESS_CLOSE_STDIN);
		assertEqual (all, ph.wait());
	}
	{
		Pipe inPipe;
		Pipe errPipe;
		ProcessHandle ph = Process::launch(cmd, args, &inPipe, nullptr, &errPipe, Poco::PROCESS_CLOSE_STDOUT);
		assertEqual (all, ph.wait());
	}
	{
		Pipe inPipe;
		Pipe outPipe;
		ProcessHandle ph = Process::launch(cmd, args, &inPipe, &outPipe, nullptr, Poco::PROCESS_CLOSE_STDERR);
		assertEqual (all, ph.wait());
	}
	{
		ProcessHandle ph = Process::launch(cmd, args,
			Poco::PROCESS_CLOSE_STDIN | Poco::PROCESS_CLOSE_STDOUT | Poco::PROCESS_CLOSE_STDERR);
		assertEqual (all, ph.wait());
	}
#endif
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


void ProcessTest::testWaitAfterKill()
{
	// The handle of a killed process can be waited for, as for a process
	// that ended on its own.
	Process::Args args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(getFullName("TestApp"), args, &inPipe, nullptr, nullptr);
	Process::kill(ph);
	const int rc = ph.wait();
#if defined(POCO_OS_FAMILY_UNIX)
	assertEqual (256 + SIGKILL, rc);
#else
	// kill() passes the exit code 0 to TerminateProcess().
	assertEqual (0, rc);
#endif
	assertTrue (!Process::isRunning(ph));
}


void ProcessTest::testKillAfterExit()
{
	// Killing a process that has ended but has not been waited for does nothing.
	Process::Args args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(getFullName("TestApp"), args, &inPipe, nullptr, nullptr);
	Process::kill(ph);
	// Process::isRunning() with a PID does not reap the process on POSIX.
	Poco::Timestamp waitStart;
	while (Process::isRunning(ph.id()))
	{
		if (waitStart.isElapsed(30 * Poco::Timestamp::resolution())) fail("Process did not terminate within 30 seconds");
		Thread::sleep(100);
	}
	Process::kill(ph);
	const int rc = ph.wait();
#if defined(POCO_OS_FAMILY_UNIX)
	assertEqual (256 + SIGKILL, rc);
#else
	assertEqual (0, rc);
#endif
}


void ProcessTest::testKillAfterExitWithCodeStillActive()
{
	// On Windows, a process that has ended with the exit code 259 has the
	// exit code that GetExitCodeProcess() gives for a running process
	// (STILL_ACTIVE); killing it does nothing, as for any ended process.
#if defined(POCO_OS_FAMILY_WINDOWS)
	Process::Args args;
	args.push_back("-count");
	Pipe inPipe;
	ProcessHandle ph = Process::launch(getFullName("TestApp"), args, &inPipe, nullptr, nullptr);
	PipeOutputStream ostr(inPipe);
	ostr << std::string(259, 'x');
	ostr.close();
	assertEqual (259, ph.wait());
	Process::kill(ph);
#endif
}


namespace
{
	std::string testApp()
	{
		std::string name("TestApp");
#if defined(_DEBUG) && (POCO_OS != POCO_OS_ANDROID)
		name += "d";
#endif
		return name;
	}


	bool appears(const std::string& path, int seconds)
	{
		Poco::Stopwatch sw;
		sw.start();
		while (!Poco::File(path).exists())
		{
			if (sw.elapsedSeconds() > seconds) return false;
			Thread::sleep(10);
		}
		return true;
	}
}


void ProcessTest::testRequestTerminationOfNewGroup()
{
#if POCO_OS != POCO_OS_ANDROID
	// A child in a process group of its own is asked to end the way a
	// console program understands: CTRL_BREAK on Windows, SIGINT on POSIX.
	const std::string ready = Poco::TemporaryFile::tempName();
	Process::Args args;
	args.push_back("-wait-break");
	args.push_back(ready);
	ProcessHandle ph = Process::launch(testApp(), args, Poco::PROCESS_NEW_GROUP);
	if (!appears(ready, 30))
	{
		Process::kill(ph);
		fail("the child does not wait for the request to end");
	}
	Process::requestTermination(ph.id(), Poco::PROCESS_NEW_GROUP);
	assertEqual (42, ph.wait());
	Poco::File(ready).remove();
#endif
}


void ProcessTest::testRequestTerminationWithoutConsole()
{
#if POCO_OS != POCO_OS_ANDROID
	// The same from a parent without a console, which on Windows attaches to
	// the console of the child for the moment it takes to send the event.
	const std::string ready = Poco::TemporaryFile::tempName();
	Process::Args args;
	args.push_back("-break-own-child");
	args.push_back(ready);
	ProcessHandle ph = Process::launch(testApp(), args);
	assertEqual (42, ph.wait());
	Poco::File(ready).remove();
#endif
}


void ProcessTest::testRequestTerminationKeepsStandardHandles()
{
#if POCO_OS != POCO_OS_ANDROID
	// The same from a parent started without inherited standard handles,
	// whose standard handles attaching to a console replaces on Windows:
	// they are its own again once the event is sent.
	const std::string ready = Poco::TemporaryFile::tempName();
	Process::Args args;
	args.push_back("-break-own-child-without-std-handles");
	args.push_back(ready);
	ProcessHandle ph = Process::launch(testApp(), args);
	assertEqual (42, ph.wait());
	Poco::File(ready).remove();
#endif
}


void ProcessTest::testInvalidPid()
{
#if POCO_OS != POCO_OS_ANDROID
	// A PID of 0 or below names a process group on POSIX. The child checks it
	// from a process group of its own, which a signal sent anyway would reach
	// alone.
	Process::Args args;
	args.push_back("-signal-invalid-pids");
	ProcessHandle ph = Process::launch(testApp(), args, Poco::PROCESS_NEW_GROUP);
	assertEqual (42, ph.wait());
#endif
}


void ProcessTest::testRequestTerminationOfGroupZero()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	// Process group 0 stands for every process on the console of the caller.
	try
	{
		Process::requestTermination(0, Poco::PROCESS_NEW_GROUP);
		fail("process group 0 must be rejected");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
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
	CppUnit_addTest(pSuite, ProcessTest, testLaunchClearedEnvironment);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchEnvKeepsParentEnvironment);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchEnvReplacesParentVariable);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchArgs);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchInvalidCommand);
	CppUnit_addTest(pSuite, ProcessTest, testIsRunning);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchCloseHandles);
	CppUnit_addTest(pSuite, ProcessTest, testLaunchClosedHandlesStayTaken);
	CppUnit_addTest(pSuite, ProcessTest, testIsRunningAllowsForTermination);
	CppUnit_addTest(pSuite, ProcessTest, testIsRunningByPidAllowsForTermination);
	CppUnit_addTest(pSuite, ProcessTest, testWaitAfterIsRunning);
	CppUnit_addTest(pSuite, ProcessTest, testConcurrentWaitAndIsRunning);
	CppUnit_addTest(pSuite, ProcessTest, testSignalExitCode);
	CppUnit_addTest(pSuite, ProcessTest, testIsRunningByPidThenWaitOnHandle);
	CppUnit_addTest(pSuite, ProcessTest, testIsRunningAfterWait);
	CppUnit_addTest(pSuite, ProcessTest, testWaitAfterKill);
	CppUnit_addTest(pSuite, ProcessTest, testKillAfterExit);
	CppUnit_addTest(pSuite, ProcessTest, testKillAfterExitWithCodeStillActive);
	CppUnit_addTest(pSuite, ProcessTest, testRequestTerminationOfNewGroup);
	CppUnit_addTest(pSuite, ProcessTest, testRequestTerminationWithoutConsole);
	CppUnit_addTest(pSuite, ProcessTest, testRequestTerminationKeepsStandardHandles);
	CppUnit_addTest(pSuite, ProcessTest, testInvalidPid);
	CppUnit_addTest(pSuite, ProcessTest, testRequestTerminationOfGroupZero);

	return pSuite;
}
