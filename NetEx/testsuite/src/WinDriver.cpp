//
// WinDriver.cpp
//
// $Id: //poco/1.4/Net/testsuite/src/WinDriver.cpp#1 $
//
// Windows test driver for Poco Net.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "WinTestRunner/WinTestRunner.h"
#include "NetExTestSuite.h"


class TestDriver: public CppUnit::WinTestRunnerApp
{
	void TestMain()
	{
		CppUnit::WinTestRunner runner;
		runner.addTest(NetExTestSuite::suite());
		runner.run();
	}
};


TestDriver theDriver;
