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


void PatternFormatterTest::testExtractBasename()
{
	PatternFormatter fmt;
	fmt.setProperty("pattern", "%O");
	std::string result;

	// Test Unix-style paths (should work on all platforms)
	Message msg1("src", "text", Message::PRIO_INFORMATION, "/path/to/file.cpp", 1);
	fmt.format(msg1, result);
	assertTrue (result == "file.cpp");

	result.clear();
	Message msg2("src", "text", Message::PRIO_INFORMATION, "/very/long/path/to/source/file/TestFile.cpp", 42);
	fmt.format(msg2, result);
	assertTrue (result == "TestFile.cpp");

	// Test filename only (no path)
	result.clear();
	Message msg3("src", "text", Message::PRIO_INFORMATION, "file.cpp", 1);
	fmt.format(msg3, result);
	assertTrue (result == "file.cpp");

	// Test empty path
	result.clear();
	Message msg4("src", "text", Message::PRIO_INFORMATION, "", 1);
	fmt.format(msg4, result);
	assertTrue (result == "");

	// Test null path
	result.clear();
	Message msg5("src", "text", Message::PRIO_INFORMATION, nullptr, 1);
	fmt.format(msg5, result);
	assertTrue (result == "");

	// Test path ending with separator
	result.clear();
	Message msg6("src", "text", Message::PRIO_INFORMATION, "/path/to/dir/", 1);
	fmt.format(msg6, result);
	assertTrue (result == "");

	// Test root path
	result.clear();
	Message msg7("src", "text", Message::PRIO_INFORMATION, "/", 1);
	fmt.format(msg7, result);
	assertTrue (result == "");

#if defined(_WIN32)
	// On Windows, backslash is also a separator
	result.clear();
	Message msg8("src", "text", Message::PRIO_INFORMATION, "C:\\path\\to\\file.cpp", 1);
	fmt.format(msg8, result);
	assertTrue (result == "file.cpp");

	result.clear();
	Message msg9("src", "text", Message::PRIO_INFORMATION, "C:\\path/mixed\\separators/file.cpp", 1);
	fmt.format(msg9, result);
	assertTrue (result == "file.cpp");
#else
	// On Unix, backslash is a valid filename character, not a separator
	result.clear();
	Message msg8("src", "text", Message::PRIO_INFORMATION, "/path/to/fi\\le.cpp", 1);
	fmt.format(msg8, result);
	assertTrue (result == "fi\\le.cpp");

	result.clear();
	Message msg9("src", "text", Message::PRIO_INFORMATION, "/path/file\\with\\backslashes.cpp", 1);
	fmt.format(msg9, result);
	assertTrue (result == "file\\with\\backslashes.cpp");
#endif
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
	CppUnit_addTest(pSuite, PatternFormatterTest, testExtractBasename);

	return pSuite;
}
