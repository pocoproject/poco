//
// ProcessRunnerTest.h
//
// Definition of the ProcessRunnerTest class.
//
// Copyright (c) 2023, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering d.o.o.,
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//



#ifndef ProcessRunnerTest_INCLUDED
#define ProcessRunnerTest_INCLUDED


#include "CppUnit/TestCase.h"
#include "Poco/ProcessRunner.h"
#include "Poco/Stopwatch.h"


class ProcessRunnerTest: public CppUnit::TestCase
{
public:
	ProcessRunnerTest(const std::string& name);
	~ProcessRunnerTest();
	
	void testPIDFile();
	void testProcessRunner();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	std::string cmdLine(const std::string& cmd, const Poco::ProcessRunner::Args& args);
	void checkTimeout(const Poco::Stopwatch& sw, const std::string& msg, int timeoutMS, Poco::LineNumber line);
};


#endif // ProcessRunnerTest_INCLUDED
