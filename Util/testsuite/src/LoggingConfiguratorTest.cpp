//
// LoggingConfiguratorTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "LoggingConfiguratorTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/LoggingConfigurator.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/LoggingRegistry.h"
#include "Poco/ConsoleChannel.h"
#if defined(_WIN32)
#include "Poco/WindowsConsoleChannel.h"
#endif
#include "Poco/FileChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Logger.h"
#include "Poco/Message.h"
#include "Poco/AutoPtr.h"
#include <sstream>


using Poco::Util::LoggingConfigurator;
using Poco::Util::PropertyFileConfiguration;
using Poco::LoggingRegistry;
using Poco::Channel;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::SplitterChannel;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::Logger;
using Poco::Message;
using Poco::AutoPtr;


LoggingConfiguratorTest::LoggingConfiguratorTest(const std::string& name): CppUnit::TestCase(name)
{
}


LoggingConfiguratorTest::~LoggingConfiguratorTest()
{
}


void LoggingConfiguratorTest::testConfigurator()
{
	static const std::string config =
		"logging.loggers.root.channel = c1\n"
		"logging.loggers.root.level = warning\n"
		"logging.loggers.l1.name = logger1\n"
		"logging.loggers.l1.channel.class = FileChannel\n"
		"logging.loggers.l1.channel.pattern = %s: [%p] %t\n"
		"logging.loggers.l1.channel.path = logfile.log\n"
		"logging.loggers.l1.level = information\n"
		"logging.loggers.l2.name = logger2\n"
		"logging.loggers.l2.channel.class = SplitterChannel\n"
		"logging.loggers.l2.channel.channel1 = c2\n"
		"logging.loggers.l2.channel.channel2 = c3\n"
		"logging.loggers.l2.level = debug\n"
		"logging.channels.c1.class = ConsoleChannel\n"
		"logging.channels.c1.formatter = f1\n"
		"logging.channels.c2.class = FileChannel\n"
		"logging.channels.c2.path = ${system.tempDir}/sample.log\n"
		"logging.channels.c2.formatter.class = PatternFormatter\n"
		"logging.channels.c2.formatter.pattern = %s: {%p} %t\n"
		"logging.channels.c3.class = ConsoleChannel\n"
		"logging.channels.c3.pattern = %s: [%p] %t\n"
		"logging.formatters.f1.class = PatternFormatter\n"
		"logging.formatters.f1.pattern = %s-[%p] %t\n"
		"logging.formatters.f1.times = UTC\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	configurator.configure(pConfig);

	Logger& root = Logger::get("");
	assertTrue (root.getLevel() == Message::PRIO_WARNING);
	FormattingChannel::Ptr pFC = root.getChannel().cast<FormattingChannel>();
	assertNotNull (pFC);
#if defined(_WIN32)
	assertTrue (!pFC->getChannel().cast<Poco::WindowsConsoleChannel>().isNull());
#else
	assertTrue (!pFC->getChannel().cast<Poco::ConsoleChannel>().isNull());
#endif
	assertTrue (!pFC->getFormatter().cast<Poco::PatternFormatter>().isNull());
	assertTrue ((pFC->getFormatter().cast<PatternFormatter>())->getProperty("pattern") == "%s-[%p] %t");

	Logger& logger1 = Logger::get("logger1");
	assertTrue (logger1.getLevel() == Message::PRIO_INFORMATION);
	pFC = logger1.getChannel().cast<FormattingChannel>();
	assertTrue (!pFC.isNull());
	assertTrue (!pFC->getChannel().cast<FileChannel>().isNull());
	assertTrue (!pFC->getFormatter().cast<PatternFormatter>().isNull());
	assertTrue (pFC->getFormatter().cast<PatternFormatter>()->getProperty("pattern") == "%s: [%p] %t");

	Logger& logger2 = Logger::get("logger2");
	assertTrue (logger2.getLevel() == Message::PRIO_DEBUG);
	assertTrue (!logger2.getChannel().cast<SplitterChannel>().isNull());
}


void LoggingConfiguratorTest::testBadConfiguration1()
{
	// this is mainly testing for memory leaks in case of
	// a bad configuration.

	static const std::string config =
		"logging.loggers.root.channel = c1\n"
		"logging.loggers.root.level = warning\n"
		"logging.loggers.l1.name = logger1\n"
		"logging.loggers.l1.channel.class = FileChannel\n"
		"logging.loggers.l1.channel.pattern = %s: [%p] %t\n"
		"logging.loggers.l1.channel.path = logfile.log\n"
		"logging.loggers.l1.level = information\n"
		"logging.loggers.l2.name = logger2\n"
		"logging.loggers.l2.channel.class = UnknownChannel\n"
		"logging.loggers.l2.level = debug\n"
		"logging.channels.c1.class = ConsoleChannel\n"
		"logging.channels.c1.formatter = f1\n"
		"logging.channels.c2.class = FileChannel\n"
		"logging.channels.c2.path = ${system.tempDir}/sample.log\n"
		"logging.channels.c2.formatter.class = PatternFormatter\n"
		"logging.channels.c2.formatter.pattern = %s: {%p} %t\n"
		"logging.channels.c3.class = ConsoleChannel\n"
		"logging.channels.c3.pattern = %s: [%p] %t\n"
		"logging.formatters.f1.class = PatternFormatter\n"
		"logging.formatters.f1.pattern = %s-[%p] %t\n"
		"logging.formatters.f1.times = UTC\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	try
	{
		configurator.configure(pConfig);
		fail("bad configuration - must throw");
	}
	catch (Poco::Exception&)
	{
	}
}


void LoggingConfiguratorTest::testBadConfiguration2()
{
	// this is mainly testing for memory leaks in case of
	// a bad configuration.

	static const std::string config =
		"logging.loggers.root.channel = c1\n"
		"logging.loggers.root.level = unknown\n"
		"logging.loggers.l1.name = logger1\n"
		"logging.loggers.l1.channel.class = FileChannel\n"
		"logging.loggers.l1.channel.pattern = %s: [%p] %t\n"
		"logging.loggers.l1.channel.path = logfile.log\n"
		"logging.loggers.l1.level = information\n"
		"logging.loggers.l2.name = logger2\n"
		"logging.loggers.l2.channel.class = SplitterChannel\n"
		"logging.loggers.l2.level = debug\n"
		"logging.channels.c1.class = ConsoleChannel\n"
		"logging.channels.c1.formatter = f1\n"
		"logging.channels.c2.class = FileChannel\n"
		"logging.channels.c2.path = ${system.tempDir}/sample.log\n"
		"logging.channels.c2.formatter.class = PatternFormatter\n"
		"logging.channels.c2.formatter.pattern = %s: {%p} %t\n"
		"logging.channels.c3.class = ConsoleChannel\n"
		"logging.channels.c3.pattern = %s: [%p] %t\n"
		"logging.formatters.f1.class = PatternFormatter\n"
		"logging.formatters.f1.pattern = %s-[%p] %t\n"
		"logging.formatters.f1.times = UTC\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	try
	{
		configurator.configure(pConfig);
		fail("bad configuration - must throw");
	}
	catch (Poco::Exception&)
	{
	}
}


void LoggingConfiguratorTest::testBadConfiguration3()
{
	// this is mainly testing for memory leaks in case of
	// a bad configuration.

	static const std::string config =
		"logging.loggers.root.channel = c1\n"
		"logging.loggers.root.level = warning\n"
		"logging.loggers.l1.name = logger1\n"
		"logging.loggers.l1.channel.class = FileChannel\n"
		"logging.loggers.l1.channel.pattern = %s: [%p] %t\n"
		"logging.loggers.l1.channel.path = logfile.log\n"
		"logging.loggers.l1.level = information\n"
		"logging.loggers.l2.name = logger2\n"
		"logging.loggers.l2.channel.class = UnknownChannel\n"
		"logging.loggers.l2.level = debug\n"
		"logging.channels.c1.class = ConsoleChannel\n"
		"logging.channels.c1.formatter = f1\n"
		"logging.channels.c2.class = FileChannel\n"
		"logging.channels.c2.path = ${system.tempDir}/sample.log\n"
		"logging.channels.c2.formatter.class = UnknownFormatter\n"
		"logging.channels.c2.formatter.pattern = %s: {%p} %t\n"
		"logging.channels.c3.class = ConsoleChannel\n"
		"logging.channels.c3.pattern = %s: [%p] %t\n"
		"logging.formatters.f1.class = PatternFormatter\n"
		"logging.formatters.f1.pattern = %s-[%p] %t\n"
		"logging.formatters.f1.times = UTC\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	try
	{
		configurator.configure(pConfig);
		fail("bad configuration - must throw");
	}
	catch (Poco::Exception&)
	{
	}
}


void LoggingConfiguratorTest::testBadConfiguration4()
{
	// this is mainly testing for memory leaks in case of
	// a bad configuration.

	static const std::string config =
		"logging.loggers.root.channel = c1\n"
		"logging.loggers.root.level = warning\n"
		"logging.loggers.l1.name = logger1\n"
		"logging.loggers.l1.channel.class = FileChannel\n"
		"logging.loggers.l1.channel.pattern = %s: [%p] %t\n"
		"logging.loggers.l1.channel.path = logfile.log\n"
		"logging.loggers.l1.level = information\n"
		"logging.loggers.l2.name = logger2\n"
		"logging.loggers.l2.channel.class = UnknownChannel\n"
		"logging.loggers.l2.level = debug\n"
		"logging.channels.c1.class = ConsoleChannel\n"
		"logging.channels.c1.formatter = f1\n"
		"logging.channels.c2.class = FileChannel\n"
		"logging.channels.c2.path = ${system.tempDir}/sample.log\n"
		"logging.channels.c2.formatter.class = PatternFormatter\n"
		"logging.channels.c2.formatter.pattern = %s: {%p} %t\n"
		"logging.channels.c2.formatter.unknown = FOO\n"
		"logging.channels.c3.class = ConsoleChannel\n"
		"logging.channels.c3.pattern = %s: [%p] %t\n"
		"logging.formatters.f1.class = PatternFormatter\n"
		"logging.formatters.f1.pattern = %s-[%p] %t\n"
		"logging.formatters.f1.times = UTC\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	try
	{
		configurator.configure(pConfig);
		fail("bad configuration - must throw");
	}
	catch (Poco::Exception&)
	{
	}
}


void LoggingConfiguratorTest::setUp()
{
	LoggingRegistry::defaultRegistry().clear();
}


void LoggingConfiguratorTest::tearDown()
{
}


CppUnit::Test* LoggingConfiguratorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LoggingConfiguratorTest");

	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testConfigurator);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testBadConfiguration1);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testBadConfiguration2);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testBadConfiguration3);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testBadConfiguration4);

	return pSuite;
}
