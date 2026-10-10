//
// FastLoggerTest.h
//
// Definition of the FastLoggerTest class.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef FastLoggerTest_INCLUDED
#define FastLoggerTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class FastLoggerTest: public CppUnit::TestCase
{
public:
	FastLoggerTest(const std::string& name);
	~FastLoggerTest();

	void testLogger();
	void testFormat();
	void testFormatAny();
	void testDump();
	void testRotateBySize();
	void testRotateDaily();
	void testRotateLargeFile();
	void testRotateWithPurgeCount();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
};


#endif // FastLoggerTest_INCLUDED
