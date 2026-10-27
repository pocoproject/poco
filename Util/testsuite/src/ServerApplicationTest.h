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
#endif

	void setUp() override;
		/// Does nothing.

	void tearDown() override;
		/// Does nothing.

	static CppUnit::Test* suite();
		/// Returns the suite of all tests of this class.
};


#endif // ServerApplicationTest_INCLUDED
