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
#include "Poco/TemporaryFile.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <thread>
#include <vector>
#if defined(POCO_OS_FAMILY_UNIX)
#include <unistd.h>
#include <fcntl.h>
#endif


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


void LoggingConfiguratorTest::testFastLoggerRootPocoLoggerWritesFile()
{
	// Root logger is type=fast with a splitter (console + file). Messages logged
	// through Poco::Logger (not FastLogger directly) must reach the configured file:
	// the type=fast branch must install the FastLogger as the Poco logger's channel.
#ifdef POCO_ENABLE_FASTLOGGER
	Poco::TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::replace(path.begin(), path.end(), '\\', '/');  // property value uses forward slashes

	const std::string config =
		"logging.loggers.root.type = fast\n"
		"logging.loggers.root.channel = splitter\n"
		"logging.loggers.root.level = debug\n"
		"logging.formatters.localFormatter.class = PatternFormatter\n"
		"logging.formatters.localFormatter.pattern = %H:%M:%S.%i [%p] %s<%I>: %t\n"
		"logging.formatters.localFormatter.times = local\n"
		"logging.channels.file.class = FileChannel\n"
		"logging.channels.file.formatter = localFormatter\n"
		"logging.channels.file.times = local\n"
		"logging.channels.file.path = " + path + "\n"
		"logging.channels.file.rotation = daily\n"
		"logging.channels.file.archive = number\n"
		"logging.channels.file.purgeCount = 10\n"
		"logging.channels.console.class = ColorConsoleChannel\n"
		"logging.channels.console.formatter = localFormatter\n"
		"logging.channels.splitter.class = SplitterChannel\n"
		"logging.channels.splitter.channels = console, file\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	configurator.configure(pConfig);

	Logger& appLogger = Logger::get("MCITest.Component");
	appLogger.information("message routed via Poco::Logger");

	// Drain anything queued by the FastLogger backend.
	Poco::FastLogger::root().flush();
	Poco::FastLogger::shutdown();

	Poco::File f(path);
	assertTrue (f.exists());
	assertTrue (f.getSize() > 0);  // message must reach the FastLogger file sink

	// The %s field must show the originating Poco logger name, not the FastLogger's
	// internal Quill logger name.
	Poco::FileInputStream fis(path);
	std::string line;
	std::getline(fis, line);
	fis.close();
	assertTrue (line.find("MCITest.Component") != std::string::npos);
	assertTrue (line.find("message routed via Poco::Logger") != std::string::npos);

	try { f.remove(); } catch (...) {}
#endif
}


void LoggingConfiguratorTest::testFastLoggerPerLoggerTraceIndependence()
{
	// Per-logger trace levels must work under type=fast. With the root FastLogger at
	// information, a child explicitly set to trace must emit its trace, while loggers
	// inheriting the root level must not (nor the root's own trace). The per-logger
	// Poco level is the sole gate; the FastLogger channel must not re-gate on the
	// root's single level.
#ifdef POCO_ENABLE_FASTLOGGER
	Poco::TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::replace(path.begin(), path.end(), '\\', '/');

	const std::string config =
		"logging.loggers.root.type = fast\n"
		"logging.loggers.root.channel = file\n"
		"logging.loggers.root.level = information\n"
		"logging.loggers.cpp.name = PerLoggerTest.Cpp\n"
		"logging.loggers.cpp.level = trace\n"
		"logging.formatters.fmt.class = PatternFormatter\n"
		"logging.formatters.fmt.pattern = [%p] %s: %t\n"
		"logging.channels.file.class = FileChannel\n"
		"logging.channels.file.formatter = fmt\n"
		"logging.channels.file.path = " + path + "\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;
	configurator.configure(pConfig);

	// Child explicitly at trace: its trace is emitted.
	Logger::get("PerLoggerTest.Cpp").trace("CPP_TRACE_LINE");
	// Sibling inheriting root (information): its trace is dropped, its info kept.
	Logger::get("PerLoggerTest.Other").trace("OTHER_TRACE_LINE");
	Logger::get("PerLoggerTest.Other").information("OTHER_INFO_LINE");
	// Root at information: its own trace is dropped.
	Logger::root().trace("ROOT_TRACE_LINE");

	Poco::FastLogger::root().flush();
	Poco::FastLogger::shutdown();

	Poco::File f(path);
	assertTrue (f.exists());

	Poco::FileInputStream fis(path);
	std::string content, line;
	while (std::getline(fis, line)) content += line + "\n";
	fis.close();

	// Child at trace emits, tagged with its own logger name (%s).
	assertTrue (content.find("CPP_TRACE_LINE") != std::string::npos);
	assertTrue (content.find("PerLoggerTest.Cpp") != std::string::npos);
	// Sibling info emits; the root level is unaffected by raising the child to trace.
	assertTrue (content.find("OTHER_INFO_LINE") != std::string::npos);
	// The sibling's trace and the root's own trace are gated out per-logger.
	assertTrue (content.find("OTHER_TRACE_LINE") == std::string::npos);
	assertTrue (content.find("ROOT_TRACE_LINE") == std::string::npos);

	try { f.remove(); } catch (...) {}
#endif
}


void LoggingConfiguratorTest::testFastLoggerFlushDrainsPerSourceLoggers()
{
	// FastLogger::flush() must drain the per-source Quill loggers used by the type=fast
	// bridge, not just the base logger. A caller that flushes without shutting the
	// backend down must still see bridged messages on disk. shutdown()'s Backend::stop()
	// flushes everything, so it would mask a flush() that misses the per-source loggers.
#ifdef POCO_ENABLE_FASTLOGGER
	Poco::TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::replace(path.begin(), path.end(), '\\', '/');

	const std::string config =
		"logging.loggers.root.type = fast\n"
		"logging.loggers.root.channel = file\n"
		"logging.loggers.root.level = debug\n"
		"logging.channels.file.class = FileChannel\n"
		"logging.channels.file.path = " + path + "\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);
	LoggingConfigurator configurator;
	configurator.configure(pConfig);

	// Log via Poco::Logger -> the FastLogger bridge -> a per-source Quill logger
	// named "FlushTest.Source", not the base _pQuillLogger.
	Logger& appLogger = Logger::get("FlushTest.Source");
	appLogger.information("drained by flush without shutdown");

	// Capture the file content after flush() but before shutdown(), so the assertion
	// reflects flush() alone. If flush() missed the per-source logger, the file would
	// still be empty here and only shutdown()'s Backend::stop() would write it.
	Poco::FastLogger::root().flush();
	Poco::File f(path);
	std::string flushed;
	if (f.exists())
	{
		Poco::FileInputStream fis(path);
		std::string line;
		while (std::getline(fis, line)) flushed += line + "\n";
		fis.close();
	}

	Poco::FastLogger::shutdown();

	// flush() alone drained the per-source logger: both the message text and the
	// originating source name are on disk.
	assertTrue (flushed.find("drained by flush without shutdown") != std::string::npos);
	assertTrue (flushed.find("FlushTest.Source") != std::string::npos);
	try { f.remove(); } catch (...) {}
#endif
}


void LoggingConfiguratorTest::testFastLoggerMultipleSourcesRouted()
{
	// The type=fast bridge must route messages from distinct Poco::Logger sources to
	// their own per-source Quill logger, so %s shows each originating name rather than
	// collapsing every source onto one logger name.
#ifdef POCO_ENABLE_FASTLOGGER
	Poco::TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::replace(path.begin(), path.end(), '\\', '/');

	const std::string config =
		"logging.loggers.root.type = fast\n"
		"logging.loggers.root.channel = file\n"
		"logging.loggers.root.level = debug\n"
		"logging.formatters.fmt.class = PatternFormatter\n"
		"logging.formatters.fmt.pattern = [%p] %s: %t\n"
		"logging.channels.file.class = FileChannel\n"
		"logging.channels.file.formatter = fmt\n"
		"logging.channels.file.path = " + path + "\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);
	LoggingConfigurator configurator;
	configurator.configure(pConfig);

	Logger::get("Multi.Alpha").information("ALPHA_MESSAGE");
	Logger::get("Multi.Beta").information("BETA_MESSAGE");

	Poco::FastLogger::root().flush();
	Poco::FastLogger::shutdown();

	Poco::File f(path);
	assertTrue (f.exists());
	Poco::FileInputStream fis(path);
	std::string content, line;
	while (std::getline(fis, line)) content += line + "\n";
	fis.close();

	// Both messages present, each attributed to its own source (%s).
	assertTrue (content.find("Multi.Alpha") != std::string::npos);
	assertTrue (content.find("ALPHA_MESSAGE") != std::string::npos);
	assertTrue (content.find("Multi.Beta") != std::string::npos);
	assertTrue (content.find("BETA_MESSAGE") != std::string::npos);

	// The alpha line carries the alpha source, not beta's: no source collapse.
	const std::string::size_type alphaPos = content.find("ALPHA_MESSAGE");
	std::string::size_type lineStart = content.rfind('\n', alphaPos);
	lineStart = (lineStart == std::string::npos) ? 0 : lineStart + 1;
	const std::string alphaLine = content.substr(lineStart, content.find('\n', alphaPos) - lineStart);
	assertTrue (alphaLine.find("Multi.Alpha") != std::string::npos);
	assertTrue (alphaLine.find("Multi.Beta") == std::string::npos);

	try { f.remove(); } catch (...) {}
#endif
}


void LoggingConfiguratorTest::testFastLoggerReconfigureRoutesToNewSinks()
{
	// After a reconfigure, a message from an already-seen source must reach the new
	// sinks, not the previous configuration's. Quill's frontend never releases loggers
	// and keys them by name, so unless setChannel() removes the prior per-source
	// loggers, the bridge keeps writing to the old file. The backend must stay up
	// between configs (flush, not shutdown) so the removal completes.
#ifdef POCO_ENABLE_FASTLOGGER
	Poco::TemporaryFile tempFile1;
	Poco::TemporaryFile tempFile2;
	std::string path1 = tempFile1.path();
	std::string path2 = tempFile2.path();
	std::replace(path1.begin(), path1.end(), '\\', '/');
	std::replace(path2.begin(), path2.end(), '\\', '/');

	const std::string fmt =
		"logging.formatters.fmt.class = PatternFormatter\n"
		"logging.formatters.fmt.pattern = [%p] %s: %t\n";
	const std::string config1 =
		"logging.loggers.root.type = fast\n"
		"logging.loggers.root.channel = file\n"
		"logging.loggers.root.level = debug\n" + fmt +
		"logging.channels.file.class = FileChannel\n"
		"logging.channels.file.formatter = fmt\n"
		"logging.channels.file.path = " + path1 + "\n";
	const std::string config2 =
		"logging.loggers.root.type = fast\n"
		"logging.loggers.root.channel = file\n"
		"logging.loggers.root.level = debug\n" + fmt +
		"logging.channels.file.class = FileChannel\n"
		"logging.channels.file.formatter = fmt\n"
		"logging.channels.file.path = " + path2 + "\n";

	{
		std::istringstream istr(config1);
		AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);
		LoggingConfigurator configurator;
		configurator.configure(pConfig);
	}
	// Create the per-source logger for "Recfg.Source" under config 1.
	Logger::get("Recfg.Source").information("BEFORE_RECONFIG");
	Poco::FastLogger::root().flush();   // flush, not shutdown: backend stays up

	{
		std::istringstream istr(config2);
		AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);
		LoggingConfigurator configurator;
		configurator.configure(pConfig);   // reconfigures the same root FastLogger
	}
	// Same source as before, but must now write to file 2, not the stale file 1.
	Logger::get("Recfg.Source").information("AFTER_RECONFIG");
	Poco::FastLogger::root().flush();
	Poco::FastLogger::shutdown();

	auto readAll = [](const std::string& p)
	{
		std::string out, line;
		Poco::File f(p);
		if (f.exists())
		{
			Poco::FileInputStream fis(p);
			while (std::getline(fis, line)) out += line + "\n";
			fis.close();
		}
		return out;
	};
	const std::string c1 = readAll(path1);
	const std::string c2 = readAll(path2);

	assertTrue (c2.find("AFTER_RECONFIG") != std::string::npos);   // new sinks took effect
	assertTrue (c1.find("AFTER_RECONFIG") == std::string::npos);   // stale per-source logger was released

	try { Poco::File(path1).remove(); } catch (...) {}
	try { Poco::File(path2).remove(); } catch (...) {}
#endif
}


void LoggingConfiguratorTest::testFastLoggerConcurrentSourcesRouted()
{
	// Several threads log concurrently through distinct Poco::Logger sources into one
	// type=fast file config. After flush every thread's messages must be present.
	// Exercises the per-source bridge map and the atomic field reads under contention.
#ifdef POCO_ENABLE_FASTLOGGER
	Poco::TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::replace(path.begin(), path.end(), '\\', '/');

	const std::string config =
		"logging.loggers.root.type = fast\n"
		"logging.loggers.root.channel = file\n"
		"logging.loggers.root.level = debug\n"
		"logging.formatters.fmt.class = PatternFormatter\n"
		"logging.formatters.fmt.pattern = [%p] %s: %t\n"
		"logging.channels.file.class = FileChannel\n"
		"logging.channels.file.formatter = fmt\n"
		"logging.channels.file.path = " + path + "\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);
	LoggingConfigurator configurator;
	configurator.configure(pConfig);

	const int kThreads = 8;
	const int kPerThread = 50;
	std::vector<std::thread> threads;
	for (int t = 0; t < kThreads; ++t)
	{
		threads.emplace_back([t]() {
			Logger& lg = Logger::get("Conc.Source" + std::to_string(t));
			for (int i = 0; i < kPerThread; ++i)
				lg.information("msg_" + std::to_string(t) + "_" + std::to_string(i));
		});
	}
	for (auto& th: threads) th.join();

	Poco::FastLogger::root().flush();
	Poco::FastLogger::shutdown();

	Poco::File f(path);
	assertTrue (f.exists());
	Poco::FileInputStream fis(path);
	std::string content, line;
	while (std::getline(fis, line)) content += line + "\n";
	fis.close();

	// Each thread's source tag and its first and last message survived the race.
	for (int t = 0; t < kThreads; ++t)
	{
		const std::string s = std::to_string(t);
		assertTrue (content.find("Conc.Source" + s) != std::string::npos);
		assertTrue (content.find("msg_" + s + "_0") != std::string::npos);
		assertTrue (content.find("msg_" + s + "_" + std::to_string(kPerThread - 1)) != std::string::npos);
	}
	try { f.remove(); } catch (...) {}
#endif
}


void LoggingConfiguratorTest::testFastLoggerPerLoggerLevelWithSplitter()
{
	// Per-logger level filtering must apply consistently across all sinks of a
	// splitter. Root at information with a splitter of two files: a child raised to
	// trace reaches both files; an inheriting sibling's trace reaches neither, its
	// info reaches both.
#ifdef POCO_ENABLE_FASTLOGGER
	Poco::TemporaryFile tf1, tf2;
	std::string p1 = tf1.path(), p2 = tf2.path();
	std::replace(p1.begin(), p1.end(), '\\', '/');
	std::replace(p2.begin(), p2.end(), '\\', '/');

	const std::string config =
		"logging.loggers.root.type = fast\n"
		"logging.loggers.root.channel = splitter\n"
		"logging.loggers.root.level = information\n"
		"logging.loggers.cpp.name = SplitLvl.Cpp\n"
		"logging.loggers.cpp.level = trace\n"
		"logging.formatters.fmt.class = PatternFormatter\n"
		"logging.formatters.fmt.pattern = [%p] %s: %t\n"
		"logging.channels.f1.class = FileChannel\n"
		"logging.channels.f1.formatter = fmt\n"
		"logging.channels.f1.path = " + p1 + "\n"
		"logging.channels.f2.class = FileChannel\n"
		"logging.channels.f2.formatter = fmt\n"
		"logging.channels.f2.path = " + p2 + "\n"
		"logging.channels.splitter.class = SplitterChannel\n"
		"logging.channels.splitter.channels = f1, f2\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);
	LoggingConfigurator configurator;
	configurator.configure(pConfig);

	Logger::get("SplitLvl.Cpp").trace("CHILD_TRACE");          // child at trace -> both files
	Logger::get("SplitLvl.Other").trace("SIBLING_TRACE");      // inherits info -> neither
	Logger::get("SplitLvl.Other").information("SIBLING_INFO");  // -> both files

	Poco::FastLogger::root().flush();
	Poco::FastLogger::shutdown();

	auto readAll = [](const std::string& p)
	{
		std::string out, line;
		Poco::File f(p);
		if (f.exists())
		{
			Poco::FileInputStream fis(p);
			while (std::getline(fis, line)) out += line + "\n";
			fis.close();
		}
		return out;
	};
	const std::string c1 = readAll(p1);
	const std::string c2 = readAll(p2);

	assertTrue (c1.find("CHILD_TRACE") != std::string::npos);
	assertTrue (c2.find("CHILD_TRACE") != std::string::npos);
	assertTrue (c1.find("SIBLING_INFO") != std::string::npos);
	assertTrue (c2.find("SIBLING_INFO") != std::string::npos);
	assertTrue (c1.find("SIBLING_TRACE") == std::string::npos);
	assertTrue (c2.find("SIBLING_TRACE") == std::string::npos);

	try { Poco::File(p1).remove(); } catch (...) {}
	try { Poco::File(p2).remove(); } catch (...) {}
#endif
}


void LoggingConfiguratorTest::testFastLoggerMalformedBackendOption()
{
	// A malformed numeric quill.* option must not abort backend startup. The option is
	// parsed lazily in ensureBackendStarted() on first logger creation, so throwing
	// there would surface from an unrelated get()/log(); it must be skipped with a
	// warning instead.
#ifdef POCO_ENABLE_FASTLOGGER
	Poco::FastLogger::shutdown();   // force the backend to restart and re-parse options
	const std::string config =
		"logging.loggers.l1.name = fastlogger_badopt\n"
		"logging.loggers.l1.type = fast\n"
		"logging.loggers.l1.channel.class = NullChannel\n"
		"logging.loggers.l1.level = debug\n"
		"logging.loggers.l1.quill.sleepDuration = not_a_number\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);
	LoggingConfigurator configurator;
	try
	{
		configurator.configure(pConfig);   // starts the backend and parses the options
		Poco::FastLogger::get("fastlogger_badopt").information("survives a bad option");
		Poco::FastLogger::root().flush();
	}
	catch (const Poco::Exception& e)
	{
		failmsg(std::string("malformed backend option must not throw: ") + e.displayText());
	}
	Poco::FastLogger::shutdown();
#endif
}


void LoggingConfiguratorTest::testFastLoggerEventChannelOnlyNoConsole()
{
	// An EventChannel-only channel yields no Quill sink (FastLogger has no EventChannel
	// analogue, so collectSinksFromChannel skips it). Such a logger must route to a null
	// sink, not the constructor-default console sink, so it must not write to the
	// console. On Unix we redirect stdout to a file and assert nothing lands there;
	// elsewhere we at least exercise the zero-sink path end to end.
#ifdef POCO_ENABLE_FASTLOGGER
	const std::string config =
		"logging.loggers.root.type = fast\n"
		"logging.loggers.root.channel = eventonly\n"
		"logging.loggers.root.level = debug\n"
		"logging.channels.eventonly.class = EventChannel\n";

	std::istringstream istr(config);
	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);
	LoggingConfigurator configurator;
	configurator.configure(pConfig);

#if defined(POCO_OS_FAMILY_UNIX)
	Poco::TemporaryFile capture;
	const std::string capturePath = capture.path();
	std::fflush(stdout);
	const int savedStdout = ::dup(STDOUT_FILENO);
	const int captureFd = ::open(capturePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	const bool redirected = (savedStdout >= 0 && captureFd >= 0 && ::dup2(captureFd, STDOUT_FILENO) >= 0);
	if (captureFd >= 0) ::close(captureFd);

	Logger& appLogger = Logger::get("EventOnlyTest.Source");
	appLogger.information("must NOT reach the console");
	appLogger.error("also suppressed");
	Poco::FastLogger::root().flush();
	Poco::FastLogger::shutdown();   // drain all output before restoring stdout

	std::fflush(stdout);
	if (savedStdout >= 0) { ::dup2(savedStdout, STDOUT_FILENO); ::close(savedStdout); }

	if (redirected)
	{
		Poco::File cf(capturePath);
		assertTrue (cf.exists() && cf.getSize() == 0);   // null sink: no console output
	}
	try { Poco::File(capturePath).remove(); } catch (...) {}
#else
	Logger& appLogger = Logger::get("EventOnlyTest.Source");
	appLogger.information("goes to the null sink, not the console");
	appLogger.error("also suppressed");
	Poco::FastLogger::root().flush();
	Poco::FastLogger::shutdown();
	// stdout capture is POSIX-specific; reaching here exercises the zero-sink path.
	assertTrue (true);
#endif
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

	// Now call getLogger with a different config -- should be ignored
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
	// Config was skipped due to collision -- logger inherits from parent (root = warning)
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
	// Reset the logger hierarchy as well as the registry: a type=fast logger installs
	// the FastLogger as the Poco logger's channel, and a possibly shut-down FastLogger
	// channel must not leak into a later test.
	Logger::shutdown();
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
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerRootPocoLoggerWritesFile);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerPerLoggerTraceIndependence);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerFlushDrainsPerSourceLoggers);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerMultipleSourcesRouted);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerReconfigureRoutesToNewSinks);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerConcurrentSourcesRouted);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerPerLoggerLevelWithSplitter);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerMalformedBackendOption);
	CppUnit_addTest(pSuite, LoggingConfiguratorTest, testFastLoggerEventChannelOnlyNoConsole);
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
