//
// ServerApplicationTest.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ServerApplicationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_FORK_EXEC) && POCO_OS != POCO_OS_ANDROID
#include "Poco/NumberFormatter.h"
#include "Poco/Path.h"
#include "Poco/Pipe.h"
#include "Poco/PipeStream.h"
#include "Poco/Process.h"
#include "Poco/StreamCopier.h"
#include "Poco/TemporaryFile.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#endif


#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_FORK_EXEC) && POCO_OS != POCO_OS_ANDROID
using namespace std::string_literals;


namespace
{
	[[nodiscard]] std::vector<std::string> readLines(const std::string& path)
		/// Returns the lines of the file at path, or an empty vector
		/// if the file cannot be opened.
	{
		std::vector<std::string> lines;
		std::ifstream istr(path);
		std::string line;
		while (std::getline(istr, line))
		{
			lines.push_back(line);
		}
		return lines;
	}
}
#endif


ServerApplicationTest::ServerApplicationTest(const std::string& name): CppUnit::TestCase(name)
{
}


ServerApplicationTest::~ServerApplicationTest() = default;


#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_FORK_EXEC) && POCO_OS != POCO_OS_ANDROID
void ServerApplicationTest::testDaemonParentSkipsExitHandlers()
{
	static constexpr auto DAEMON_EXIT_TIMEOUT = std::chrono::seconds(10);
	static constexpr auto POLL_INTERVAL = std::chrono::milliseconds(10);

	std::string cmd("DaemonTestApp");
#if defined(_DEBUG)
	cmd += "d";
#endif
	Poco::TemporaryFile marker;
	const std::string markerPath = Poco::Path(marker.path()).absolute().toString();
	const Poco::Process::Args args{"--daemon"s, markerPath};
	Poco::Pipe outPipe;
	Poco::ProcessHandle ph = Poco::Process::launch(cmd, args, nullptr, &outPipe, nullptr);
	Poco::PipeInputStream istr(outPipe);
	std::string output;
	Poco::StreamCopier::copyToString(istr, output);
	const int rc = ph.wait();
	if (rc != 0)
	{
		failmsg("DaemonTestApp did not start or failed, exit code " + Poco::NumberFormatter::format(rc));
	}
	const std::string parentPid = Poco::NumberFormatter::format(ph.id());

	// The daemon appends its own process ID when it exits. Wait for it, so that
	// it does not create the marker again after the test removed it.
	std::vector<std::string> markerPids;
	bool daemonExited = false;
	const auto deadline = std::chrono::steady_clock::now() + DAEMON_EXIT_TIMEOUT;
	while (!daemonExited && std::chrono::steady_clock::now() < deadline)
	{
		markerPids = readLines(markerPath);
		daemonExited = std::any_of(markerPids.begin(), markerPids.end(),
			[&parentPid](const std::string& pid) { return pid != parentPid; });
		if (!daemonExited) std::this_thread::sleep_for(POLL_INTERVAL);
	}

	assertEqual ("before daemon\n"s, output);
	assertTrue (daemonExited);
	assertEqual (1, markerPids.size());
}
#endif


void ServerApplicationTest::setUp()
{
}


void ServerApplicationTest::tearDown()
{
}


CppUnit::Test* ServerApplicationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ServerApplicationTest");

#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_FORK_EXEC) && POCO_OS != POCO_OS_ANDROID
	CppUnit_addTest(pSuite, ServerApplicationTest, testDaemonParentSkipsExitHandlers);
#endif

	return pSuite;
}
