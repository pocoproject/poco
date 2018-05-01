//
// PatternFormatterTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PatternFormatterTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Message.h"
#include "Poco/DateTime.h"
#include "Poco/Exception.h"


using Poco::PatternFormatter;
using Poco::Message;
using Poco::DateTime;


PatternFormatterTest::PatternFormatterTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


PatternFormatterTest::~PatternFormatterTest()
{
}


void PatternFormatterTest::testPatternFormatter()
{
	Message msg;
	PatternFormatter fmt;
	msg.setSource("TestSource");
	msg.setText("Test message text");
	msg.setPid(1234);
	msg.setTid(1);
	msg.setThread("TestThread");
	msg.setPriority(Message::PRIO_ERROR);
	msg.setTime(DateTime(2005, 1, 1, 14, 30, 15, 500).timestamp());
	msg["testParam"] = "Test Parameter";
	
	std::string result;
	fmt.setProperty("pattern", "%Y-%m-%dT%H:%M:%S [%s] %p: %t");
	fmt.format(msg, result);
	assertTrue (result == "2005-01-01T14:30:15 [TestSource] Error: Test message text");
	
	result.clear();
	fmt.setProperty("pattern", "%w, %e %b %y %H:%M:%S.%i [%s:%I:%T] %q: %t");
	fmt.format(msg, result);
	assertTrue (result == "Sat, 1 Jan 05 14:30:15.500 [TestSource:1:TestThread] E: Test message text");

	result.clear();
	fmt.setProperty("pattern", "%Y-%m-%d %H:%M:%S [%N:%P:%s]%l-%t");
	fmt.format(msg, result);
	assertTrue (result.find("2005-01-01 14:30:15 [") == 0);
	assertTrue (result.find(":TestSource]3-Test message text") != std::string::npos);
	
	result.clear();
	assertTrue (fmt.getProperty("times") == "UTC");
	fmt.setProperty("times", "local");
	fmt.format(msg, result);
	assertTrue (result.find("2005-01-01 ") == 0);
	assertTrue (result.find(":TestSource]3-Test message text") != std::string::npos);
	
	result.clear();
	fmt.setProperty("pattern", "%[testParam]");
	fmt.format(msg, result);
	assertTrue (result == "Test Parameter");

	result.clear();
	fmt.setProperty("pattern", "%[testParam] %p");
	fmt.format(msg, result);
	assertTrue (result == "Test Parameter Error");

	result.clear();
	fmt.setProperty("pattern", "start %v[10] end");
	fmt.format(msg, result);
	assertTrue (result == "start TestSource end");

	result.clear();
	fmt.setProperty("pattern", "start %v[12] end");
	fmt.format(msg, result);
	assertTrue (result == "start TestSource   end");

	result.clear();
	fmt.setProperty("pattern", "start %v[8] end");
	fmt.format(msg, result);
	assertTrue (result == "start stSource end");
	
	result.clear();
	fmt.setProperty("pattern", "%p");
	fmt.setProperty("priorityNames", "FAT, CRI, ERR, WRN, NTC, INF, DBG, TRC");
	fmt.format(msg, result);
	assertTrue (result == "ERR");
	
	try
	{
		fmt.setProperty("priorityNames", "FAT, CRI,");
		fail("invalid value, must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}
}


void PatternFormatterTest::setUp()
{
}


void PatternFormatterTest::tearDown()
{
}


CppUnit::Test* PatternFormatterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PatternFormatterTest");

	CppUnit_addTest(pSuite, PatternFormatterTest, testPatternFormatter);

	return pSuite;
}
