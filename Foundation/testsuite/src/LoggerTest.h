//
// LoggerTest.h
//
// Definition of the LoggerTest class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef LoggerTest_INCLUDED
#define LoggerTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"
#include <string>

class LoggerTest: public CppUnit::TestCase
{
public:
	LoggerTest(const std::string& name);
	~LoggerTest();

	void testLogger();
	void testFormat();
	void testFormatAny();
	void testDump();
	void testFormatThreadName();
	void testFormatStdThreadName();
	void testLoggerRefSurvivesShutdown();
	void testConcurrentChannelReplacement();
	void testConcurrentShutdown();
	void testLogDuringDestructionOfReplaced();
	void testGetDuringDestructionOfDetached();
	void testConcurrentSetLevel();
	void testReplacedChannelReleased();
	void testReplaceChannelWithMessageOnItsWay();
	void testReplaceFormatterWithMessageOnItsWay();
	void testReplacedChannelNotKeptByLaterMessage();
	void testChannelReplacesItself();
	void testLogWhileThreadEnds();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	template <typename ThreadFactory>
	std::string doTestFormatThreadName(ThreadFactory makeThread);
};


#endif // LoggerTest_INCLUDED
