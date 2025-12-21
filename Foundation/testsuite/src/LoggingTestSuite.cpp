//
// LoggingTestSuite.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Foundation.h"
#include "LoggingTestSuite.h"
#include "LoggerTest.h"
#ifdef POCO_ENABLE_FASTLOGGER
#include "FastLoggerTest.h"
#include "FastLoggerChannelsTest.h"
#endif
#include "ChannelTest.h"
#include "PatternFormatterTest.h"
#include "JSONFormatterTest.h"
#include "FileChannelTest.h"
#include "SimpleFileChannelTest.h"
#include "LoggingFactoryTest.h"
#include "LoggingRegistryTest.h"
#include "LogStreamTest.h"


CppUnit::Test* LoggingTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LoggingTestSuite");

	pSuite->addTest(LoggerTest::suite());
#ifdef POCO_ENABLE_FASTLOGGER
	pSuite->addTest(FastLoggerTest::suite());
	pSuite->addTest(FastLoggerChannelsTest::suite());
#endif
	pSuite->addTest(ChannelTest::suite());
	pSuite->addTest(PatternFormatterTest::suite());
	pSuite->addTest(JSONFormatterTest::suite());
	pSuite->addTest(FileChannelTest::suite());
	pSuite->addTest(SimpleFileChannelTest::suite());
	pSuite->addTest(LoggingFactoryTest::suite());
	pSuite->addTest(LoggingRegistryTest::suite());
	pSuite->addTest(LogStreamTest::suite());

	return pSuite;
}
