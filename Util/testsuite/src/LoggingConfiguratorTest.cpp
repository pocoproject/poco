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


void LoggingConfiguratorTest::testGetLogger()
{
	static const std::string config =
		"logging.channels.gl1ch.class = ConsoleChannel\n"
		"logging.channels.gl1ch.pattern = %s: [%p] %t\n"
		"logging.loggers.l1.name = getLoggerTest\n"
		"logging.loggers.l1.channel = gl1ch\n"
		"logging.loggers.l1.level = debug\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	Logger& logger = configurator.getLogger("getLoggerTest", pConfig);
	assertTrue (logger.name() == "getLoggerTest");
	assertTrue (logger.getLevel() == Message::PRIO_DEBUG);
	FormattingChannel::Ptr pFC = logger.getChannel().cast<FormattingChannel>();
	assertNotNull (pFC);
#if defined(_WIN32)
	assertTrue (!pFC->getChannel().cast<Poco::WindowsConsoleChannel>().isNull());
#else
	assertTrue (!pFC->getChannel().cast<Poco::ConsoleChannel>().isNull());
#endif
}


void LoggingConfiguratorTest::testGetLoggerExisting()
{
	// Pre-create the logger via configure()
	static const std::string initialConfig =
		"logging.channels.glech.class = ConsoleChannel\n"
		"logging.channels.glech.pattern = %s: [%p] %t\n"
		"logging.loggers.l1.name = existingLogger\n"
		"logging.loggers.l1.channel = glech\n"
		"logging.loggers.l1.level = warning\n";

	std::istringstream istr1(initialConfig);
	AutoPtr<PropertyFileConfiguration> pInitialConfig = new PropertyFileConfiguration(istr1);

	LoggingConfigurator configurator;
	configurator.configure(pInitialConfig);

	Logger& existing = Logger::get("existingLogger");
	assertTrue (existing.getLevel() == Message::PRIO_WARNING);

	// Now call getLogger with a different config — should be ignored
	static const std::string newConfig =
		"logging.loggers.l1.name = existingLogger\n"
		"logging.loggers.l1.level = debug\n";

	std::istringstream istr2(newConfig);
	AutoPtr<PropertyFileConfiguration> pNewConfig = new PropertyFileConfiguration(istr2);

	Logger& logger = configurator.getLogger("existingLogger", pNewConfig);
	assertTrue (&logger == &existing);
	assertTrue (logger.getLevel() == Message::PRIO_WARNING); // unchanged
}


void LoggingConfiguratorTest::testGetLoggerCollision()
{
	// Register a channel in the registry first
	static const std::string initialConfig =
		"logging.channels.sharedCh.class = ConsoleChannel\n"
		"logging.loggers.root.channel = sharedCh\n"
		"logging.loggers.root.level = warning\n";

	std::istringstream istr1(initialConfig);
	AutoPtr<PropertyFileConfiguration> pInitialConfig = new PropertyFileConfiguration(istr1);

	LoggingConfigurator configurator;
	configurator.configure(pInitialConfig);

	// Now try getLogger with a config that redefines "sharedCh"
	static const std::string collidingConfig =
		"logging.channels.sharedCh.class = FileChannel\n"
		"logging.channels.sharedCh.path = collision.log\n"
		"logging.loggers.l1.name = collisionLogger\n"
		"logging.loggers.l1.channel = sharedCh\n"
		"logging.loggers.l1.level = debug\n";

	std::istringstream istr2(collidingConfig);
	AutoPtr<PropertyFileConfiguration> pCollidingConfig = new PropertyFileConfiguration(istr2);

	Logger& logger = configurator.getLogger("collisionLogger", pCollidingConfig);
	assertTrue (logger.name() == "collisionLogger");
	// Config was skipped due to collision — logger inherits from parent (root = warning)
	assertTrue (logger.getLevel() == Message::PRIO_WARNING);
	// Verify channel is NOT a FileChannel (collision config was skipped)
	assertTrue (logger.getChannel().cast<FileChannel>().isNull());
}


void LoggingConfiguratorTest::testConfigureLogger()
{
	static const std::string config = R"(
logging.formatters.scriptFmt.class = PatternFormatter
logging.formatters.scriptFmt.pattern = %s: [%p] %t
logging.channels.scriptFile.class = ConsoleChannel
logging.channels.scriptSplitter.class = SplitterChannel
logging.channels.scriptSplitter.channels = scriptFile
logging.loggers.script.name = scriptLogger
logging.loggers.script.channel = scriptSplitter
logging.loggers.script.level = debug
)";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	configurator.configure(pConfig, "script");

	Logger& logger = Logger::get("scriptLogger");
	assertTrue (logger.name() == "scriptLogger");
	assertTrue (logger.getLevel() == Message::PRIO_DEBUG);
}


void LoggingConfiguratorTest::testReconfigureLogger()
{
	// Initial configuration: ConsoleChannel
	static const std::string config1 = R"(
logging.formatters.myFmt.class = PatternFormatter
logging.formatters.myFmt.pattern = %s: [%p] %t
logging.channels.myFile.class = ConsoleChannel
logging.channels.mySplitter.class = SplitterChannel
logging.channels.mySplitter.channels = myFile
logging.loggers.my.name = myLogger
logging.loggers.my.channel = mySplitter
logging.loggers.my.level = information
)";

	std::istringstream istr1(config1);
	AutoPtr<PropertyFileConfiguration> pConfig1 = new PropertyFileConfiguration(istr1);

	LoggingConfigurator configurator;
	configurator.configure(pConfig1, "my");

	Logger& logger = Logger::get("myLogger");
	assertTrue (logger.getLevel() == Message::PRIO_INFORMATION);

	// Reconfigure: change level to debug
	static const std::string config2 = R"(
logging.formatters.myFmt.class = PatternFormatter
logging.formatters.myFmt.pattern = %s: [%p] %t (updated)
logging.channels.myFile.class = ConsoleChannel
logging.channels.mySplitter.class = SplitterChannel
logging.channels.mySplitter.channels = myFile
logging.loggers.my.name = myLogger
logging.loggers.my.channel = mySplitter
logging.loggers.my.level = debug
)";

	std::istringstream istr2(config2);
	AutoPtr<PropertyFileConfiguration> pConfig2 = new PropertyFileConfiguration(istr2);

	configurator.configure(pConfig2, "my");

	// Logger should be reconfigured with new level
	assertTrue (logger.getLevel() == Message::PRIO_DEBUG);
}


void LoggingConfiguratorTest::testGetLoggerNoFormattersNoChannels()
{
	// Config with only loggers, no formatters or channels.
	// validateConfiguration() must accept this (return true).
	static const std::string config = R"(
logging.loggers.l1.name = loggerOnlyTest
logging.loggers.l1.level = debug
)";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	Logger& logger = configurator.getLogger("loggerOnlyTest", pConfig);
	assertTrue (logger.name() == "loggerOnlyTest");
	assertTrue (logger.getLevel() == Message::PRIO_DEBUG);
}


void LoggingConfiguratorTest::testConfigureLoggerNestedSplitter()
{
	// Test deeply nested SplitterChannel traversal via collectChannelNames.
	static const std::string config = R"(
logging.formatters.nestedFmt.class = PatternFormatter
logging.formatters.nestedFmt.pattern = %s: [%p] %t
logging.channels.leaf1.class = ConsoleChannel
logging.channels.leaf2.class = ConsoleChannel
logging.channels.innerSplit.class = SplitterChannel
logging.channels.innerSplit.channels = leaf1, leaf2
logging.channels.outerSplit.class = SplitterChannel
logging.channels.outerSplit.channels = innerSplit
logging.channels.outerSplit.formatter = nestedFmt
logging.loggers.nested.name = nestedSplitLogger
logging.loggers.nested.channel = outerSplit
logging.loggers.nested.level = trace
)";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	configurator.configure(pConfig, "nested");

	Logger& logger = Logger::get("nestedSplitLogger");
	assertTrue (logger.name() == "nestedSplitLogger");
	assertTrue (logger.getLevel() == Message::PRIO_TRACE);
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
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testGetLogger);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testGetLoggerExisting);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testGetLoggerCollision);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testGetLoggerNoFormattersNoChannels);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testConfigureLogger);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testConfigureLoggerNestedSplitter);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testReconfigureLogger);

	return pSuite;
}
