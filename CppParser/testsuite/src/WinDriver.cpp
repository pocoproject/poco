//
// WinDriver.cpp
//
// $Id: //poco/1.3/CppParser/testsuite/src/WinDriver.cpp#1 $
//
// Windows test driver for Poco CppParser.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "WinTestRunner/WinTestRunner.h"
#include "CppParserTestSuite.h"


class TestDriver: public CppUnit::WinTestRunnerApp
{
	void TestMain()
	{
		CppUnit::WinTestRunner runner;
		runner.addTest(CppParserTestSuite::suite());
		runner.run();
	}
};


TestDriver theDriver;
