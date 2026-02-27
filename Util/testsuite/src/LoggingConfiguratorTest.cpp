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
#ifdef POCO_ENABLE_FASTLOGGER
#include "Poco/FastLogger.h"
#endif
#include "Poco/NullChannel.h"
#include "Poco/AsyncChannel.h"
#include "Poco/Util/LoggingSubsystem.h"
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


void LoggingConfiguratorTest::testFastLoggerConfigurator()
{
	// Test configuration with type=fast for FastLogger
	static const std::string config =
		"logging.loggers.root.channel = c1\n"
		"logging.loggers.root.level = warning\n"
		"logging.loggers.l1.name = fastlogger1\n"
		"logging.loggers.l1.type = fast\n"
		"logging.loggers.l1.channel.class = ConsoleChannel\n"
		"logging.loggers.l1.level = debug\n"
		"logging.loggers.l2.name = standardlogger1\n"
		"logging.loggers.l2.channel.class = ConsoleChannel\n"
		"logging.loggers.l2.level = information\n"
		"logging.channels.c1.class = ConsoleChannel\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;

#ifdef POCO_ENABLE_FASTLOGGER
	configurator.configure(pConfig);

	// Verify FastLogger was created with correct level
	Poco::FastLogger& fastLogger = Poco::FastLogger::get("fastlogger1");
	assertTrue (fastLogger.getLevel() == Message::PRIO_DEBUG);
	assertTrue (fastLogger.name() == "fastlogger1");

	// Verify standard Logger was also created
	Logger& stdLogger = Logger::get("standardlogger1");
	assertTrue (stdLogger.getLevel() == Message::PRIO_INFORMATION);

	// Verify root logger (standard) was created
	Logger& root = Logger::get("");
	assertTrue (root.getLevel() == Message::PRIO_WARNING);

	// Clean up FastLogger
	Poco::FastLogger::shutdown();
#else
	// When FastLogger is not enabled, type=fast should throw
	try
	{
		configurator.configure(pConfig);
		failmsg("FastLogger not enabled - must throw InvalidAccessException");
	}
	catch (Poco::InvalidAccessException&)
	{
		// Expected
	}
#endif
}


void LoggingConfiguratorTest::testFastLoggerRootConfigurator()
{
	// Test configuration with type=fast for root logger
	static const std::string config =
		"logging.loggers.root.type = fast\n"
		"logging.loggers.root.channel.class = NullChannel\n"
		"logging.loggers.root.level = notice\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;

#ifdef POCO_ENABLE_FASTLOGGER
	configurator.configure(pConfig);

	// Verify FastLogger root was created with correct level
	Poco::FastLogger& rootLogger = Poco::FastLogger::root();
	assertTrue (rootLogger.getLevel() == Message::PRIO_NOTICE);
	assertTrue (rootLogger.name().empty());

	// Verify it can log
	rootLogger.notice("Test message from root FastLogger");
	rootLogger.flush();

	// Clean up FastLogger
	Poco::FastLogger::shutdown();
#else
	// When FastLogger is not enabled, type=fast should throw
	try
	{
		configurator.configure(pConfig);
		failmsg("FastLogger not enabled - must throw InvalidAccessException");
	}
	catch (Poco::InvalidAccessException&)
	{
		// Expected
	}
#endif
}


void LoggingConfiguratorTest::testFastLoggerWithRegistry()
{
	// Test FastLogger configuration using channel reference from LoggingRegistry
	// (channel defined in logging.channels, referenced by name in logging.loggers)
	static const std::string config =
		"logging.loggers.l1.name = fastlogger_registry\n"
		"logging.loggers.l1.type = fast\n"
		"logging.loggers.l1.channel = nullchan\n"
		"logging.loggers.l1.level = debug\n"
		"logging.channels.nullchan.class = NullChannel\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;

#ifdef POCO_ENABLE_FASTLOGGER
	configurator.configure(pConfig);

	// Verify FastLogger was created with correct level
	Poco::FastLogger& fastLogger = Poco::FastLogger::get("fastlogger_registry");
	assertTrue (fastLogger.getLevel() == Message::PRIO_DEBUG);
	assertTrue (fastLogger.name() == "fastlogger_registry");

	// Verify it can log (channel was set from registry)
	fastLogger.debug("Test message using channel from registry");
	fastLogger.flush();

	// Clean up FastLogger
	Poco::FastLogger::shutdown();
#else
	// When FastLogger is not enabled, type=fast should throw
	try
	{
		configurator.configure(pConfig);
		failmsg("FastLogger not enabled - must throw InvalidAccessException");
	}
	catch (Poco::InvalidAccessException&)
	{
		// Expected
	}
#endif
}


void LoggingConfiguratorTest::testFastLoggerOptions()
{
	// Test configuration with quill.* options for FastLogger
	static const std::string config =
		"logging.loggers.l1.name = fastlogger_quill\n"
		"logging.loggers.l1.type = fast\n"
		"logging.loggers.l1.channel.class = NullChannel\n"
		"logging.loggers.l1.level = debug\n"
		"logging.loggers.l1.quill.threadName = TestQuillBackend\n"
		"logging.loggers.l1.quill.sleepDuration = 500\n"
		"logging.loggers.l1.quill.enableYieldWhenIdle = true\n"
		"logging.loggers.l1.quill.transitEventBufferInitialCapacity = 512\n"
		"logging.loggers.l1.quill.sinkMinFlushInterval = 100\n"
		"logging.loggers.l1.quill.waitForQueuesToEmptyBeforeExit = true\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;

#ifdef POCO_ENABLE_FASTLOGGER
	// Note: setBackendOption must be called before any logging starts.
	// In this test, the configuration is processed before the backend starts,
	// so the quill.* options should be accepted without throwing.
	configurator.configure(pConfig);

	// Verify FastLogger was created with correct level
	Poco::FastLogger& fastLogger = Poco::FastLogger::get("fastlogger_quill");
	assertTrue (fastLogger.getLevel() == Message::PRIO_DEBUG);
	assertTrue (fastLogger.name() == "fastlogger_quill");

	// The quill options are applied to the backend thread - we can't easily
	// verify them programmatically, but we can verify the logger works
	fastLogger.information("Test message with quill options");
	fastLogger.flush();

	// Clean up FastLogger
	Poco::FastLogger::shutdown();
#else
	// When FastLogger is not enabled, type=fast should throw
	try
	{
		configurator.configure(pConfig);
		failmsg("FastLogger not enabled - must throw InvalidAccessException");
	}
	catch (Poco::InvalidAccessException&)
	{
		// Expected
	}
#endif
}


void LoggingConfiguratorTest::testFastLoggerOptionsWarnings()
{
	// Test that quill.* options on poco loggers produce warnings (not errors)
	// and are ignored
	static const std::string config =
		"logging.loggers.l1.name = poco_logger_with_quill_options\n"
		"logging.loggers.l1.channel.class = NullChannel\n"
		"logging.loggers.l1.level = information\n"
		"logging.loggers.l1.quill.threadName = ShouldBeIgnored\n"
		"logging.loggers.l1.quill.sleepDuration = 999\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;

	// This should NOT throw - quill options are ignored with a warning
	configurator.configure(pConfig);

	// Verify standard Logger was created correctly
	Logger& logger = Logger::get("poco_logger_with_quill_options");
	assertTrue (logger.getLevel() == Message::PRIO_INFORMATION);
	assertTrue (!logger.getChannel().cast<Poco::NullChannel>().isNull());
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
		failmsg("bad configuration - must throw");
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
		failmsg("bad configuration - must throw");
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
		failmsg("bad configuration - must throw");
	}
	catch (Poco::Exception&)
	{
	}
}


void LoggingConfiguratorTest::testAsyncChannelShutdown()
{
	// Test that LoggingSubsystem::uninitialize() properly shuts down
	// AsyncChannel without deadlocking (GitHub issue #5228).

	static const std::string config =
		"logging.loggers.root.channel = asyncChan\n"
		"logging.loggers.root.level = debug\n"
		"logging.channels.asyncChan.class = AsyncChannel\n"
		"logging.channels.asyncChan.channel = nullChan\n"
		"logging.channels.nullChan.class = NullChannel\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	configurator.configure(pConfig);

	Logger& root = Logger::get("");
	assertTrue (root.getLevel() == Message::PRIO_DEBUG);

	// Log some messages to ensure the async thread is running
	root.information("Test message 1");
	root.information("Test message 2");
	root.information("Test message 3");

	// Simulate what LoggingSubsystem::uninitialize() does.
	// This must not deadlock.
	Logger::shutdown();
	LoggingRegistry::defaultRegistry().clear();
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
		failmsg("bad configuration - must throw");
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
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerConfigurator);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerRootConfigurator);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerWithRegistry);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerOptions);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerOptionsWarnings);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testAsyncChannelShutdown);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testBadConfiguration1);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testBadConfiguration2);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testBadConfiguration3);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testBadConfiguration4);

	return pSuite;
}
