//
// WindowsTestSuite.cpp
//
// $Id: //poco/1.4/Util/testsuite/src/WindowsTestSuite.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "WindowsTestSuite.h"
#ifndef _WIN32_WCE
#include "WinRegistryTest.h"
#include "WinConfigurationTest.h"
#endif


CppUnit::Test* WindowsTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("WindowsTestSuite");

#ifndef _WIN32_WCE
	pSuite->addTest(WinRegistryTest::suite());
	pSuite->addTest(WinConfigurationTest::suite());
#endif

	return pSuite;
}
