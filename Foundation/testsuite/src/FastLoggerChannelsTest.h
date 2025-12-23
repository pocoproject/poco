//
// FastLoggerChannelsTest.h
//
// Definition of the FastLoggerChannelsTest class.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef FastLoggerChannelsTest_INCLUDED
#define FastLoggerChannelsTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class FastLoggerChannelsTest: public CppUnit::TestCase
{
public:
	FastLoggerChannelsTest(const std::string& name);
	~FastLoggerChannelsTest();

	void testNullChannel();
	void testConsoleChannel();
	void testColorConsoleChannel();
	void testFileChannel();
	void testSimpleFileChannel();
	void testStreamChannel();
	void testFormattingChannel();
	void testAsyncChannel();
	void testSplitterChannel();
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOG_CHANNEL)
	void testSyslogChannel();
#endif

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // FastLoggerChannelsTest_INCLUDED
