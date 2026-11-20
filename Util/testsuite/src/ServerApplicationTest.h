//
// ServerApplicationTest.h
//
// Definition of the ServerApplicationTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ServerApplicationTest_INCLUDED
#define ServerApplicationTest_INCLUDED


#include "Poco/Util/Util.h"
#include "CppUnit/TestCase.h"
#include <string>
#include <vector>


class ServerApplicationTest: public CppUnit::TestCase
	/// Tests of Poco::Util::ServerApplication.
{
public:
	ServerApplicationTest() = delete;
		/// Not supported; a test case needs a name.

	explicit ServerApplicationTest(const std::string& name);
		/// Creates the ServerApplicationTest.

	~ServerApplicationTest() override;
		/// Destroys the ServerApplicationTest.

#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_FORK_EXEC) && POCO_OS != POCO_OS_ANDROID
	void testDaemonParentSkipsExitHandlers();
		/// Checks that the foreground process of a daemon writes buffered
		/// output once and does not run the atexit handlers.

	void testDaemonFlushesWideOutput();
		/// Checks that the foreground process of a daemon writes output
		/// buffered in std::wcout once.
#endif

	void setUp() override;
		/// Does nothing.

	void tearDown() override;
		/// Does nothing.

	static CppUnit::Test* suite();
		/// Returns the suite of all tests of this class.

#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_FORK_EXEC) && POCO_OS != POCO_OS_ANDROID
private:
	[[nodiscard]] std::string runDaemonTestApp(const std::vector<std::string>& options);
		/// Runs DaemonTestApp with the given options and a marker file, waits
		/// until the daemon has exited, checks that the atexit handlers ran
		/// once and returns the output of the foreground process.
#endif
};


#endif // ServerApplicationTest_INCLUDED
