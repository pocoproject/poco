//
// JSONFormatterTest.cpp
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "JSONFormatterTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/JSONFormatter.h"
#include "Poco/Message.h"
#include "Poco/DateTime.h"


using Poco::JSONFormatter;
using Poco::Message;
using Poco::DateTime;


JSONFormatterTest::JSONFormatterTest(const std::string& name): CppUnit::TestCase(name)
{
}


JSONFormatterTest::~JSONFormatterTest()
{
}


void JSONFormatterTest::testJSONFormatter()
{
	Message msg;
	JSONFormatter fmt;
	msg.setSource("TestSource");
	msg.setText("Test message text");
	msg.setPid(1234);
	msg.setTid(1);
	msg.setThread("TestThread");
	msg.setPriority(Message::PRIO_ERROR);
	msg.setTime(DateTime(2005, 1, 1, 14, 30, 15, 500).timestamp());

	std::string result;
	fmt.format(msg, result);
	assertTrue (result == "{\"timestamp\":\"2005-01-01T14:30:15.500000Z\",\"source\":\"TestSource\",\"level\":\"error\",\"message\":\"Test message text\",\"thread\":1}");

	msg.setText("Multi\nline\ntext");
	result.clear();
	fmt.format(msg, result);
	assertTrue (result == "{\"timestamp\":\"2005-01-01T14:30:15.500000Z\",\"source\":\"TestSource\",\"level\":\"error\",\"message\":\"Multi\\nline\\ntext\",\"thread\":1}");

	fmt.setProperty("thread", "none");
	result.clear();
	fmt.format(msg, result);
	assertTrue (result == "{\"timestamp\":\"2005-01-01T14:30:15.500000Z\",\"source\":\"TestSource\",\"level\":\"error\",\"message\":\"Multi\\nline\\ntext\"}");

	msg.set("p1", "v1");
	result.clear();
	fmt.format(msg, result);
	assertTrue (result == "{\"timestamp\":\"2005-01-01T14:30:15.500000Z\",\"source\":\"TestSource\",\"level\":\"error\",\"message\":\"Multi\\nline\\ntext\",\"params\":{\"p1\":\"v1\"}}");

	msg.set("p2", "v2");
	result.clear();
	fmt.format(msg, result);
	assertTrue (result == "{\"timestamp\":\"2005-01-01T14:30:15.500000Z\",\"source\":\"TestSource\",\"level\":\"error\",\"message\":\"Multi\\nline\\ntext\",\"params\":{\"p1\":\"v1\",\"p2\":\"v2\"}}");

	fmt.setProperty("thread", "name");
	result.clear();
	fmt.format(msg, result);
	assertTrue (result == "{\"timestamp\":\"2005-01-01T14:30:15.500000Z\",\"source\":\"TestSource\",\"level\":\"error\",\"message\":\"Multi\\nline\\ntext\",\"thread\":\"TestThread\",\"params\":{\"p1\":\"v1\",\"p2\":\"v2\"}}");
}


void JSONFormatterTest::setUp()
{
}


void JSONFormatterTest::tearDown()
{
}


CppUnit::Test* JSONFormatterTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONFormatterTest");

	CppUnit_addTest(pSuite, JSONFormatterTest, testJSONFormatter);

	return pSuite;
}
