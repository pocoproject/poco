//
// LoggingFactoryTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "LoggingFactoryTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/LoggingFactory.h"
#include "Poco/Instantiator.h"
#include "Poco/Channel.h"
#include "Poco/ConsoleChannel.h"
#if defined(_WIN32)
#include "Poco/WindowsConsoleChannel.h"
#endif
#ifndef POCO_NO_FILECHANNEL
#include "Poco/FileChannel.h"
#include "Poco/SimpleFileChannel.h"
#endif
#include "Poco/SplitterChannel.h"
#include "Poco/Formatter.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Message.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include <memory>


using Poco::LoggingFactory;
using Poco::Channel;
using Poco::ConsoleChannel;
#ifndef POCO_NO_FILECHANNEL
using Poco::FileChannel;
using Poco::SimpleFileChannel;
#endif
using Poco::SplitterChannel;
using Poco::Formatter;
using Poco::PatternFormatter;
using Poco::Message;
using Poco::AutoPtr;
using Poco::Instantiator;


namespace
{
	class CustomChannel: public Channel
	{
	public:
		void log(const Message& msg)
		{
		}
	};

	class CustomFormatter: public Formatter
	{
		void format(const Message& msg, std::string& text)
		{
		}
	};
}


LoggingFactoryTest::LoggingFactoryTest(const std::string& name): CppUnit::TestCase(name)
{
}


LoggingFactoryTest::~LoggingFactoryTest()
{
}


void LoggingFactoryTest::testBuiltins()
{
	LoggingFactory& fact = LoggingFactory::defaultFactory();

	Channel::Ptr pConsoleChannel = fact.createChannel("ConsoleChannel");
#if defined(_WIN32)
	assertTrue (!pConsoleChannel.cast<Poco::WindowsConsoleChannel>().isNull());
#else
	assertTrue (!pConsoleChannel.cast<ConsoleChannel>().isNull());
#endif

#ifndef POCO_NO_FILECHANNEL
	Channel::Ptr pFileChannel = fact.createChannel("FileChannel");
	assertTrue (!pFileChannel.cast<FileChannel>().isNull());

	Channel::Ptr pSimpleFileChannel = fact.createChannel("SimpleFileChannel");
	assertTrue(!pSimpleFileChannel.cast<SimpleFileChannel>().isNull());
#endif

	Channel::Ptr pSplitterChannel = fact.createChannel("SplitterChannel");
	assertTrue (!pSplitterChannel.cast<SplitterChannel>().isNull());

	try
	{
		Channel::Ptr pUnknownChannel = fact.createChannel("UnknownChannel");
		fail("unknown class - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}

	Formatter::Ptr pPatternFormatter = fact.createFormatter("PatternFormatter");
	assertTrue (!pPatternFormatter.isNull());

	try
	{
		Formatter::Ptr pUnknownFormatter = fact.createFormatter("UnknownFormatter");
		fail("unknown class - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void LoggingFactoryTest::testCustom()
{
	std::unique_ptr<LoggingFactory> fact(new LoggingFactory);

	fact->registerChannelClass("CustomChannel", new Instantiator<CustomChannel, Channel>);
	fact->registerFormatterClass("CustomFormatter", new Instantiator<CustomFormatter, Formatter>);

	Channel::Ptr pCustomChannel = fact->createChannel("CustomChannel");
	assertTrue (dynamic_cast<CustomChannel*>(pCustomChannel.get()) != 0);

	Formatter::Ptr pCustomFormatter = fact->createFormatter("CustomFormatter");
	assertTrue (!pCustomFormatter.isNull());
}


void LoggingFactoryTest::setUp()
{
}


void LoggingFactoryTest::tearDown()
{
}


CppUnit::Test* LoggingFactoryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LoggingFactoryTest");

	CppUnit_addTest(pSuite, LoggingFactoryTest, testBuiltins);
	CppUnit_addTest(pSuite, LoggingFactoryTest, testCustom);

	return pSuite;
}
