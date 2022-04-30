//
// PatternFormatterTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PatternFormatterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Message.h"
#include "Poco/DateTime.h"


using Poco::PatternFormatter;
using Poco::Message;
using Poco::DateTime;


PatternFormatterTest::PatternFormatterTest(const std::string& name): CppUnit::TestCase(name)
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
	msg.setSourceFile(__FILE__);
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
	fmt.setProperty("pattern", "%O");
	fmt.format(msg, result);
	assertTrue (result == "PatternFormatterTest.cpp");

	result.clear();
	fmt.setProperty("priorityNames", "FATAL,CRITICAL,SPECIAL_ERROR_NAME,WARN,NOTICE,INFO,DEBUG,TRACE");
	fmt.setProperty("pattern", "%p");
	fmt.format(msg, result);
	assertTrue (result == "SPECIAL_ERROR_NAME");
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
