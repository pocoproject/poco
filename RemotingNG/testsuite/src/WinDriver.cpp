//
// WinDriver.cpp
//
// Windows test driver for Poco RemotingNG.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "WinTestRunner/WinTestRunner.h"
#include "RemotingTestSuite.h"


class TestDriver: public CppUnit::WinTestRunnerApp
{
	void TestMain()
	{
		CppUnit::WinTestRunner runner;
		runner.addTest(RemotingTestSuite::suite());
		runner.run();
	}
};


TestDriver theDriver;
