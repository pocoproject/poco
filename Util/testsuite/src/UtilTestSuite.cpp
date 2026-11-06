//
// UtilTestSuite.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "UtilTestSuite.h"
#include "ConfigurationTestSuite.h"
#include "OptionsTestSuite.h"
#include "ServerApplicationTest.h"
#include "TimerTestSuite.h"
#if defined(_MSC_VER)
#include "WindowsTestSuite.h"
#endif


CppUnit::Test* UtilTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("UtilTestSuite");

	pSuite->addTest(ConfigurationTestSuite::suite());
	pSuite->addTest(OptionsTestSuite::suite());
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_FORK_EXEC) && POCO_OS != POCO_OS_ANDROID
	// The suite has a test only where a daemon can be forked.
	pSuite->addTest(ServerApplicationTest::suite());
#endif
	pSuite->addTest(TimerTestSuite::suite());
#if defined(_MSC_VER)
	pSuite->addTest(WindowsTestSuite::suite());
#endif

	return pSuite;
}
