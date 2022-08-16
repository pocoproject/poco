//
// LoggerTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "LoggerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
#include "TestChannel.h"


using Poco::Logger;
using Poco::Channel;
using Poco::Message;
using Poco::AutoPtr;


LoggerTest::LoggerTest(const std::string& name): CppUnit::TestCase(name)
{
}


LoggerTest::~LoggerTest()
{
}


void LoggerTest::testLogger()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	Logger& root = Logger::root();
	root.setChannel(pChannel);

	assertTrue (root.getLevel() == Message::PRIO_INFORMATION);
	assertTrue (root.is(Message::PRIO_INFORMATION));
	assertTrue (root.fatal());
	assertTrue (root.critical());
	assertTrue (root.error());
	assertTrue (root.warning());
	assertTrue (root.notice());
	assertTrue (root.information());
	assertTrue (!root.debug());
	assertTrue (!root.trace());

	root.information("Informational message");
	assertTrue (pChannel->list().size() == 1);
	root.warning("Warning message");
	assertTrue (pChannel->list().size() == 2);
	root.debug("Debug message");
	assertTrue (pChannel->list().size() == 2);

	Logger& logger1 = Logger::get("Logger1");
	Logger& logger2 = Logger::get("Logger2");
	Logger& logger11 = Logger::get("Logger1.Logger1");
	Logger& logger12 = Logger::get("Logger1.Logger2");
	Logger& logger21 = Logger::get("Logger2.Logger1");
	Logger& logger22 = Logger::get("Logger2.Logger2");

	std::vector<std::string> loggers;
	Logger::names(loggers);
	assertTrue (loggers.size() == 7);
	assertTrue (loggers[0] == "");
	assertTrue (loggers[1] == "Logger1");
	assertTrue (loggers[2] == "Logger1.Logger1");
	assertTrue (loggers[3] == "Logger1.Logger2");
	assertTrue (loggers[4] == "Logger2");
	assertTrue (loggers[5] == "Logger2.Logger1");
	assertTrue (loggers[6] == "Logger2.Logger2");

	Logger::setLevel("Logger1", Message::PRIO_DEBUG);
	assertTrue (logger1.is(Message::PRIO_DEBUG));
	assertTrue (logger11.is(Message::PRIO_DEBUG));
	assertTrue (logger12.is(Message::PRIO_DEBUG));
	assertTrue (!logger2.is(Message::PRIO_DEBUG));
	assertTrue (!logger21.is(Message::PRIO_DEBUG));
	assertTrue (!logger22.is(Message::PRIO_DEBUG));
	assertTrue (logger11.is(Message::PRIO_INFORMATION));
	assertTrue (logger12.is(Message::PRIO_INFORMATION));
	assertTrue (logger21.is(Message::PRIO_INFORMATION));
	assertTrue (logger22.is(Message::PRIO_INFORMATION));

	Logger::setLevel("Logger2.Logger1", Message::PRIO_ERROR);
	assertTrue (logger1.is(Message::PRIO_DEBUG));
	assertTrue (logger11.is(Message::PRIO_DEBUG));
	assertTrue (logger12.is(Message::PRIO_DEBUG));
	assertTrue (!logger21.is(Message::PRIO_DEBUG));
	assertTrue (!logger22.is(Message::PRIO_DEBUG));
	assertTrue (logger11.is(Message::PRIO_INFORMATION));
	assertTrue (logger12.is(Message::PRIO_INFORMATION));
	assertTrue (logger21.is(Message::PRIO_ERROR));
	assertTrue (logger22.is(Message::PRIO_INFORMATION));

	Logger::setLevel("", Message::PRIO_WARNING);
	assertTrue (root.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger1.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger11.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger12.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger1.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger21.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger22.getLevel() == Message::PRIO_WARNING);

	AutoPtr<TestChannel> pChannel2 = new TestChannel;
	Logger::setChannel("Logger2", pChannel2);
	assertTrue (pChannel.get()  == root.getChannel().get());
	assertTrue (pChannel.get()  == logger1.getChannel().get());
	assertTrue (pChannel.get()  == logger11.getChannel().get());
	assertTrue (pChannel.get()  == logger12.getChannel().get());
	assertTrue (pChannel2.get() == logger2.getChannel().get());
	assertTrue (pChannel2.get() == logger21.getChannel().get());
	assertTrue (pChannel2.get() == logger22.getChannel().get());

	root.setLevel(Message::PRIO_TRACE);
	pChannel->list().clear();
	root.trace("trace");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_TRACE);
	pChannel->list().clear();
	root.debug("debug");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_DEBUG);
	pChannel->list().clear();
	root.information("information");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_INFORMATION);
	pChannel->list().clear();
	root.notice("notice");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_NOTICE);
	pChannel->list().clear();
	root.warning("warning");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_WARNING);
	pChannel->list().clear();
	root.error("error");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_ERROR);
	pChannel->list().clear();
	root.critical("critical");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_CRITICAL);
	pChannel->list().clear();
	root.fatal("fatal");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_FATAL);

	root.setLevel("1");
	assertTrue (root.getLevel() == Message::PRIO_FATAL);
	root.setLevel("8");
	assertTrue (root.getLevel() == Message::PRIO_TRACE);
	try
	{
		root.setLevel("0");
		assertTrue (0);
	}
	catch(Poco::InvalidArgumentException&)
	{
	}
	try
	{
		root.setLevel("9");
		assertTrue (0);
	}
	catch(Poco::InvalidArgumentException&)
	{
	}

}


void LoggerTest::testFormat()
{
	std::string str = Logger::format("$0$1", "foo", "bar");
	assertTrue (str == "foobar");
	str = Logger::format("foo$0", "bar");
	assertTrue (str == "foobar");
	str = Logger::format("the amount is $$ $0", "100");
	assertTrue (str == "the amount is $ 100");
	str = Logger::format("$0$1$2", "foo", "bar");
	assertTrue (str == "foobar");
	str = Logger::format("$foo$0", "bar");
	assertTrue (str == "$foobar");
	str = Logger::format("$0", "1");
	assertTrue (str == "1");
	str = Logger::format("$0$1", "1", "2");
	assertTrue (str == "12");
	str = Logger::format("$0$1$2", "1", "2", "3");
	assertTrue (str == "123");
	str = Logger::format("$0$1$2$3", "1", "2", "3", "4");
	assertTrue (str == "1234");
}

void LoggerTest::testFormatAny()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	Logger& root = Logger::root();
	root.setChannel(pChannel);

	root.error("%s%s", std::string("foo"), std::string("bar"));
	assertTrue (pChannel->getLastMessage().getText() == "foobar");

	root.error("foo%s", std::string("bar"));
	assertTrue (pChannel->getLastMessage().getText() == "foobar");

	root.error("the amount is %% %d", 100);
	assertTrue (pChannel->getLastMessage().getText() == "the amount is % 100");

	root.error("%d", 1);
	assertTrue (pChannel->getLastMessage().getText() == "1");

	root.error("%d%d", 1, 2);
	assertTrue (pChannel->getLastMessage().getText() == "12");

	root.error("%d%d%d", 1, 2, 3);
	assertTrue (pChannel->getLastMessage().getText() == "123");

	root.error("%d%d%d%d", 1, 2, 3, 4);
	assertTrue (pChannel->getLastMessage().getText() == "1234");

	root.error("%d%d%d%d%d", 1, 2, 3, 4, 5);
	assertTrue (pChannel->getLastMessage().getText() == "12345");

	root.error("%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6);
	assertTrue (pChannel->getLastMessage().getText() == "123456");

	root.error("%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7);
	assertTrue (pChannel->getLastMessage().getText() == "1234567");

	root.error("%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8);
	assertTrue (pChannel->getLastMessage().getText() == "12345678");

	root.error("%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9);
	assertTrue (pChannel->getLastMessage().getText() == "123456789");

	root.error("%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
	assertTrue (pChannel->getLastMessage().getText() == "12345678910");

	root.error("%d%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
	assertTrue (pChannel->getLastMessage().getText() == "1234567891011");

	root.error("%d%d%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	assertTrue (pChannel->getLastMessage().getText() == "123456789101112");

	root.error("%d%d%d%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
	assertTrue (pChannel->getLastMessage().getText() == "12345678910111213");

	root.error("%d%d%d%d%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
	assertTrue (pChannel->getLastMessage().getText() == "1234567891011121314");

	root.error("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	assertTrue (pChannel->getLastMessage().getText() == "123456789101112131415");
}


void LoggerTest::testDump()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	Logger& root = Logger::root();
	root.setChannel(pChannel);
	root.setLevel(Message::PRIO_INFORMATION);

	char buffer1[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
	root.dump("test", buffer1, sizeof(buffer1));
	assertTrue (pChannel->list().empty());

	root.setLevel(Message::PRIO_DEBUG);
	root.dump("test", buffer1, sizeof(buffer1));

	std::string msg = pChannel->list().begin()->getText();
	assertTrue (msg == "test\n0000  00 01 02 03 04 05                                 ......");
	pChannel->clear();

	char buffer2[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};
	root.dump("", buffer2, sizeof(buffer2));
	msg = pChannel->list().begin()->getText();
	assertTrue (msg == "0000  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F  ................");
	pChannel->clear();

	char buffer3[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
		0x20, 0x41, 0x42, 0x1f, 0x7f, 0x7e
	};
	root.dump("", buffer3, sizeof(buffer3));
	msg = pChannel->list().begin()->getText();
	assertTrue (msg == "0000  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F  ................\n"
	               "0010  20 41 42 1F 7F 7E                                  AB..~");
	pChannel->clear();
}


void LoggerTest::setUp()
{
	Logger::shutdown();
}


void LoggerTest::tearDown()
{
}


CppUnit::Test* LoggerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LoggerTest");

	CppUnit_addTest(pSuite, LoggerTest, testLogger);
	CppUnit_addTest(pSuite, LoggerTest, testFormat);
	CppUnit_addTest(pSuite, LoggerTest, testFormatAny);
	CppUnit_addTest(pSuite, LoggerTest, testDump);

	return pSuite;
}
