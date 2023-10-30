//
// ProcessRunnerTest.cpp
//
// Copyright (c) 2023, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering d.o.o.,
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ProcessRunnerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/PIDFile.h"
#include <sstream>
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Format.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/Environment.h"
#include <fstream>
#include <iostream>


//using namespace IoT::Devs::Util;
using namespace Poco;
using namespace Poco::Util;
//using IoT::Devs::Util::enableLogging;


ProcessRunnerTest::ProcessRunnerTest(const std::string& name): 
	CppUnit::TestCase(name)
{
	//enableLogging("debug", "%Y-%m-%d %H:%M:%S.%i [%s]: %t");
}


ProcessRunnerTest::~ProcessRunnerTest()
{
}


void ProcessRunnerTest::testPIDFile()
{
	std::string pidFile = Path::tempHome() + "test.pid";
	std::string pidFiled = Path::tempHome() + "testd.pid";

	{
		PIDFile f;
		assertTrue (f.getName().empty());
		assertTrue (f.getPID() == PIDFile::INVALID_PID);
		assertFalse (File(pidFile).exists());
		assertFalse (File(pidFiled).exists());

		f.setName(pidFile, false);
		assertTrue (f.getName() == pidFile);
		assertTrue (f.getPID() != PIDFile::INVALID_PID);
		assertTrue (File(pidFile).exists());
		assertTrue (!File(pidFiled).exists());

#ifdef _DEBUG
		f.setName(pidFile, true);
		assertTrue (f.getName() == pidFiled);
		assertTrue (f.getPID() != PIDFile::INVALID_PID);
		assertTrue (!File(pidFile).exists());
		assertTrue (File(pidFiled).exists());
#endif
	}
	assertFalse (File(pidFile).exists());
	assertFalse (File(pidFiled).exists());

	{
		PIDFile f(pidFile);
#ifdef _DEBUG
		std::string pf = pidFiled;
#else
		std::string pf = pidFile;
#endif

		assertTrue (f.getName() == pf);
		assertTrue (File(pf).exists());
		assertTrue (f.getPID() != PIDFile::INVALID_PID);

		assertTrue (f.exists());
	}
	assertFalse (File(pidFile).exists());
	assertFalse (File(pidFiled).exists());

	{
		PIDFile f(pidFile, false);
		assertTrue (f.getName() == pidFile);
		assertTrue (File(pidFile).exists());
		assertTrue (f.getPID() != PIDFile::INVALID_PID);

		assertTrue (f.exists());
	}
	assertFalse (File(pidFile).exists());
	assertFalse (File(pidFiled).exists());

	{
		PIDFile f(pidFile, true, false);
#ifdef _DEBUG
		std::string pf = pidFiled;
#else
		std::string pf = pidFile;
#endif

		assertTrue (f.getName() == pf);
		assertTrue (!File(pf).exists());
		assertTrue (f.getPID() == PIDFile::INVALID_PID);

		f.create(false);
		assertTrue (f.exists());
	}
	assertFalse (File(pidFile).exists());
	assertFalse (File(pidFiled).exists());
}


void ProcessRunnerTest::testProcessRunner()
{
	std::string base = Environment::get("POCO_BASE");
	std::string procName = "TestApp";
	std::string procPath = base + "/Foundation/testsuite";
	std::string binPath = procPath + Path::expand("/bin/$OSNAME/$OSARCH/");
	std::string cmd = binPath + procName;
#ifdef _DEBUG
	cmd += 'd';
#endif

	// non-auto start, no PID
	{
		std::vector<std::string> args;
		ProcessRunner pr(cmd, args, "", ProcessRunner::NO_OUT, 10, false);
		assertTrue (pr.cmdLine() == cmdLine(cmd, args));
		assertFalse (pr.running());
		pr.start();
		Stopwatch sw; sw.start();
		while (!pr.running())
			checkTimeout(sw, "Waiting for process to start", 1000);
		assertTrue (pr.running());
		try
		{
			pr.start();
			fail("It should not be possible to start a started process.");
		}
		catch(const Poco::InvalidAccessException&) {}
		pr.stop();
		sw.restart();
		while (pr.running())
			checkTimeout(sw, "Waiting for process to stop", 1000);
		assertFalse (pr.running());
		pr.start();
		while (!pr.running())
			checkTimeout(sw, "Waiting for process to start", 1000);
		assertTrue (pr.running());
		pr.stop();
		pr.stop(); // second stop() should be silent no-op
	}

	// non-auto start with PID
	{
		std::vector<std::string> args;
		std::string pidFile = Poco::format("run/%s.pid", procName);
		args.push_back(std::string("--pidfile=").append(pidFile));
		ProcessRunner pr(cmd, args, "", ProcessRunner::NO_OUT, 10, false);
		assertTrue (pr.cmdLine() == cmdLine(cmd, args));
		assertFalse (pr.running());
		pr.start();
		Stopwatch sw; sw.start();
		while (!pr.running())
			checkTimeout(sw, "Waiting for process to start", 1000);
		assertTrue (pr.running());
		try
		{
			pr.start();
			fail("It should not be possible to start a started process.");
		}
		catch(const Poco::InvalidAccessException&) {}
		pr.stop();
		sw.restart();
		while (pr.running())
			checkTimeout(sw, "Waiting for process to stop", 1000);
		assertFalse (pr.running());
		pr.start();
		while (!pr.running())
			checkTimeout(sw, "Waiting for process to start", 1000);
		assertTrue (pr.running());
		pr.stop();
		pr.stop(); // second stop() should be silent no-op
	}

	// autodetect PID file from the long command line argument
	{
		std::vector<std::string> args;
		std::string pidFile = Poco::format("run/%s.pid", procName);
		args.push_back(std::string("--pidfile=").append(pidFile));
		{
			ProcessRunner pr(cmd, args);
			assertTrue (pr.cmdLine() == cmdLine(cmd, args));
			assertTrue (pr.pidFile() == PIDFile::getFileName(pidFile));
			assertTrue (File(pidFile).exists());
			assertTrue (PIDFile::contains(pidFile, pr.pid()));
		}
		assertTrue (!File(pidFile).exists());
	}

	// autodetect PID file from the short command line argument
	{
		std::vector<std::string> args;
		std::string pidFile = Poco::format("run/%s.pid", procName);
		args.push_back(std::string("-p=").append(pidFile));
		{
			ProcessRunner pr(cmd, args, PIDFile::getFileName(pidFile));
			assertTrue (pr.cmdLine() == cmdLine(cmd, args));
			assertTrue (pr.pidFile() == pidFile);
			assertTrue (File(pidFile).exists());
			assertTrue (PIDFile::contains(pidFile, pr.pid()));
		}
		assertTrue (!File(pidFile).exists());
	}

	// ProcessRunner should NOT autodetect PID from command line args
	// if argument formats list is empty
	{
		std::vector<std::string> args;
		std::string pidFile = Poco::format("run/%s.pid", procName);
		args.push_back(std::string("--pidfile=").append(pidFile));
		{
			ProcessRunner pr(cmd, args, "", ProcessRunner::NO_OUT, 10, true, {});
			assertTrue (pr.cmdLine() == cmdLine(cmd, args));
			assertTrue (pr.pidFile().empty()); // ProcessRunner has no PID file

			PIDFile::getFileName(pidFile);
			Stopwatch sw; sw.start();
			while (!File(pidFile).exists())
				checkTimeout(sw, "Waiting for PID file", 1000);

			// PID file exists and is valid
			assertTrue (File(pidFile).exists());
			assertTrue (PIDFile::contains(pidFile, pr.pid()));
		}
		assertTrue (!File(pidFile).exists());
	}

	{
		std::vector<std::string> args;
		std::string pidFile = Poco::format("run/%s.pid", procName);
		args.push_back(std::string("-p=").append(pidFile));
		{
			ProcessRunner pr(cmd, args, "", ProcessRunner::NO_OUT, 10, true, {});
			assertTrue (pr.cmdLine() == cmdLine(cmd, args));
			assertTrue (pr.pidFile().empty()); // ProcessRunner has no PID file

			PIDFile::getFileName(pidFile);
			Stopwatch sw; sw.start();
			while (!File(pidFile).exists())
				checkTimeout(sw, "Waiting for PID file", 1000);

			// PID file exists and is valid
			assertTrue (File(pidFile).exists());
			assertTrue (PIDFile::contains(pidFile, pr.pid()));
		}
		assertTrue (!File(pidFile).exists());
	}

	// no PID file created at all
	{
		std::vector<std::string> args;
		std::string pidFile = Poco::format("run/%s.pid", procName);
		{
			ProcessRunner pr(cmd, args);
			assertTrue (pr.cmdLine() == cmdLine(cmd, args));
			assertTrue (pr.pidFile().empty());

			Thread::sleep(500);

			assertTrue (!File(pidFile).exists());
			assertTrue (!PIDFile::contains(pidFile, pr.pid()));
		}
		assertTrue (!File(pidFile).exists());
	}
}


std::string ProcessRunnerTest::cmdLine(const std::string& cmd, const ProcessRunner::Args& args)
{
	std::string cmdL = cmd + ' ';
	auto it = args.begin();
	auto end = args.end();
	for (; it != end;)
	{
		cmdL.append(*it);
		if (++it == end) break;
		cmdL.append(1, ' ');
	}
	return cmdL;
}


void ProcessRunnerTest::checkTimeout(const Stopwatch& sw, const std::string& msg, int timeoutMS)
{
	if (sw.elapsedSeconds()*1000 > timeoutMS)
	{
		throw Poco::TimeoutException(
			Poco::format("ProcessRunner::checkTimeout(): %s", msg));
	}
	Thread::sleep(10);
}


void ProcessRunnerTest::setUp()
{
}


void ProcessRunnerTest::tearDown()
{
}


CppUnit::Test* ProcessRunnerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ProcessRunnerTest");

	CppUnit_addTest(pSuite, ProcessRunnerTest, testPIDFile);
	CppUnit_addTest(pSuite, ProcessRunnerTest, testProcessRunner);

	return pSuite;
}