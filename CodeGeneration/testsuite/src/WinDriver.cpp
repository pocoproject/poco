//
// WinDriver.cpp
//
// Windows test driver for Poco CodeGeneration.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "WinTestRunner/WinTestRunner.h"
#include "CodeGenerationTestSuite.h"


class TestDriver: public CppUnit::WinTestRunnerApp
{
	void TestMain()
	{
		CppUnit::WinTestRunner runner;
		runner.addTest(CodeGenerationTestSuite::suite());
		runner.run();
	}
};


TestDriver theDriver;
