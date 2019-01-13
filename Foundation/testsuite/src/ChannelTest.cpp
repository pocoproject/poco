//
// ChannelTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ChannelTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/SplitterChannel.h"
#include "Poco/AsyncChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/Message.h"
#include "Poco/Formatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/StreamChannel.h"
#include "TestChannel.h"
#include <sstream>


using Poco::SplitterChannel;
using Poco::AsyncChannel;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::StreamChannel;
using Poco::Formatter;
using Poco::Message;
using Poco::AutoPtr;


class SimpleFormatter: public Formatter
{
public:
	void format(const Message& msg, std::string& text)
	{
		text = msg.getSource();
		text.append(": ");
		text.append(msg.getText());
	}
};


ChannelTest::ChannelTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


ChannelTest::~ChannelTest()
{
}


void ChannelTest::testSplitter()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	AutoPtr<SplitterChannel> pSplitter = new SplitterChannel;
	pSplitter->addChannel(pChannel);
	pSplitter->addChannel(pChannel);
	Message msg;
	pSplitter->log(msg);
	assertTrue (pChannel->list().size() == 2);
}


void ChannelTest::testAsync()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
#ifndef POCO_OS_FAMILY_WINDOWS
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(pChannel); // FIXME It is crashing here on Windows!
	pAsync->open();
	Message msg;
	pAsync->log(msg);
	pAsync->log(msg);
	pAsync->close();
	assertTrue (pChannel->list().size() == 2);
#endif
}


void ChannelTest::testFormatting()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	AutoPtr<Formatter> pFormatter = new SimpleFormatter;
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(pFormatter, pChannel);
	Message msg("Source", "Text", Message::PRIO_INFORMATION);
	pFormatterChannel->log(msg);
	assertTrue (pChannel->list().size() == 1);
	assertTrue (pChannel->list().begin()->getText() == "Source: Text");
}


void ChannelTest::testConsole()
{
	AutoPtr<ConsoleChannel> pChannel = new ConsoleChannel;
	AutoPtr<Formatter> pFormatter = new SimpleFormatter;
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(pFormatter, pChannel);
	Message msg("Source", "Text", Message::PRIO_INFORMATION);
	pFormatterChannel->log(msg);
}


void ChannelTest::testStream()
{
	std::ostringstream str;
	AutoPtr<StreamChannel> pChannel = new StreamChannel(str);
	AutoPtr<Formatter> pFormatter = new SimpleFormatter;
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(pFormatter, pChannel);
	Message msg("Source", "Text", Message::PRIO_INFORMATION);
	pFormatterChannel->log(msg);
	assertTrue (str.str().find("Source: Text") == 0);
}


void ChannelTest::setUp()
{
}


void ChannelTest::tearDown()
{
}


CppUnit::Test* ChannelTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ChannelTest");

	CppUnit_addTest(pSuite, ChannelTest, testSplitter);
	CppUnit_addTest(pSuite, ChannelTest, testAsync);
	CppUnit_addTest(pSuite, ChannelTest, testFormatting);
	CppUnit_addTest(pSuite, ChannelTest, testConsole);
	CppUnit_addTest(pSuite, ChannelTest, testStream);

	return pSuite;
}
