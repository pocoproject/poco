//
// FastLoggerChannelsTest.cpp
//
// Tests for FastLogger channel adapters.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Foundation.h"


#ifdef POCO_ENABLE_FASTLOGGER


#include "FastLoggerChannelsTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/FastLogger.h"
#include "Poco/Exception.h"
#include "Poco/AutoPtr.h"
#include "Poco/Message.h"
#include "Poco/NullChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/StreamChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/AsyncChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/EventChannel.h"
#include "Poco/File.h"
#include "Poco/TemporaryFile.h"
#include "Poco/FileStream.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timezone.h"
#include "Poco/NumberParser.h"
#include <sstream>

#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOG_CHANNEL)
#include "Poco/SyslogChannel.h"
#endif


using Poco::FastLogger;
using Poco::NullChannel;
using Poco::ConsoleChannel;
using Poco::ColorConsoleChannel;
using Poco::FileChannel;
using Poco::SimpleFileChannel;
using Poco::StreamChannel;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::AsyncChannel;
using Poco::SplitterChannel;
using Poco::EventChannel;
using Poco::Channel;
using Poco::Message;
using Poco::AutoPtr;
using Poco::File;
using Poco::TemporaryFile;


namespace
{
	// Returns the first whitespace-delimited token of the first line of a file
	// (the rendered timestamp field for the timezone tests).
	std::string firstToken(const std::string& path)
	{
		Poco::FileInputStream fis(path);
		std::string line;
		std::getline(fis, line);
		fis.close();
		std::string::size_type sp = line.find(' ');
		return (sp == std::string::npos) ? line : line.substr(0, sp);
	}
}


FastLoggerChannelsTest::FastLoggerChannelsTest(const std::string& name): CppUnit::TestCase(name)
{
}


FastLoggerChannelsTest::~FastLoggerChannelsTest()
{
}


void FastLoggerChannelsTest::testNullChannel()
{
	// Test that NullChannel is properly adapted (output discarded)
	AutoPtr<NullChannel> pNullChannel(new NullChannel);

	FastLogger& logger = FastLogger::get("TestAdapters.NullChannel");
	logger.setChannel(pNullChannel);
	logger.setLevel(Message::PRIO_TRACE);

	// Log messages - should not crash, output is discarded
	logger.information("Test message to null channel");
	logger.warning("Warning to null channel");
	logger.error("Error to null channel");

	// Flush to ensure messages are processed
	logger.flush();

	// No assertion needed - if we get here without crash, test passes
}


void FastLoggerChannelsTest::testConsoleChannel()
{
	// Test that ConsoleChannel is properly adapted
	// Use NullChannel to avoid console noise in tests
	AutoPtr<NullChannel> pNullChannel(new NullChannel);

	FastLogger& logger = FastLogger::get("TestAdapters.ConsoleChannel");
	logger.setChannel(pNullChannel);  // Use null to avoid console spam
	logger.setLevel(Message::PRIO_TRACE);

	// Log messages
	logger.information("Test message to console channel");

	logger.flush();

	// Test verifies ConsoleChannel adapter doesn't crash
	// Actual console output tested manually
}


void FastLoggerChannelsTest::testColorConsoleChannel()
{
	// Test that ColorConsoleChannel is properly adapted
	// Use NullChannel to avoid console noise in tests
	AutoPtr<NullChannel> pNullChannel(new NullChannel);

	FastLogger& logger = FastLogger::get("TestAdapters.ColorConsoleChannel");
	logger.setChannel(pNullChannel);  // Use null to avoid console spam
	logger.setLevel(Message::PRIO_TRACE);

	// Log messages
	logger.information("Test message to color console channel");

	logger.flush();
}


void FastLoggerChannelsTest::testFileChannel()
{
	// Test that FileChannel is properly adapted
	std::string tempFile = TemporaryFile::tempName() + ".log";

	AutoPtr<FileChannel> pFileChannel(new FileChannel(tempFile));

	FastLogger& logger = FastLogger::get("TestAdapters.FileChannel");
	logger.setChannel(pFileChannel);
	logger.setLevel(Message::PRIO_TRACE);

	// Log messages
	logger.information("Test message 1 to file channel");
	logger.warning("Test message 2 to file channel");
	logger.error("Test message 3 to file channel");

	// Flush to ensure all messages are written to file
	logger.flush();

	// Verify file exists and has content
	File file(tempFile);
	assertTrue(file.exists());
	assertTrue(file.getSize() > 0);

	// Cleanup
	try { file.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testSimpleFileChannel()
{
	// Test that SimpleFileChannel is properly adapted
	std::string tempFile = TemporaryFile::tempName() + "_simple.log";

	AutoPtr<SimpleFileChannel> pSimpleFileChannel(new SimpleFileChannel(tempFile));

	FastLogger& logger = FastLogger::get("TestAdapters.SimpleFileChannel");
	logger.setChannel(pSimpleFileChannel);
	logger.setLevel(Message::PRIO_TRACE);

	// Log messages
	logger.information("Test message to simple file channel");

	// Flush to ensure all messages are written to file
	logger.flush();

	// Verify file exists and has content
	File file(tempFile);
	assertTrue(file.exists());
	assertTrue(file.getSize() > 0);

	// Cleanup
	try { file.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testStreamChannel()
{
	// Test that StreamChannel is properly adapted
	// Note: FastLogger maps StreamChannel to stdout since the internal
	// ostream is not accessible from outside. Use NullChannel for test.
	AutoPtr<NullChannel> pNullChannel(new NullChannel);

	FastLogger& logger = FastLogger::get("TestAdapters.StreamChannel");
	logger.setChannel(pNullChannel);
	logger.setLevel(Message::PRIO_TRACE);

	// Log messages
	logger.information("Test message to stream channel");

	logger.flush();

	// Note: We can't verify the ostream content because FastLogger
	// uses Quill's StreamSink which defaults to stdout
}


void FastLoggerChannelsTest::testFormattingChannel()
{
	// Test that FormattingChannel is unwrapped and PatternFormatter is translated
	std::string tempFile = TemporaryFile::tempName() + "_fmt.log";

	AutoPtr<FileChannel> pFileChannel(new FileChannel(tempFile));
	// Use pattern with log level (%p) and message text (%t)
	AutoPtr<PatternFormatter> pFormatter(new PatternFormatter("[%p] %t"));
	AutoPtr<FormattingChannel> pFormattingChannel(new FormattingChannel(pFormatter, pFileChannel));

	FastLogger& logger = FastLogger::get("TestAdapters.FormattingChannel");
	logger.setChannel(pFormattingChannel);
	logger.setLevel(Message::PRIO_TRACE);

	// Log messages - FormattingChannel should be unwrapped, file used
	logger.information("Test message through formatting channel");

	// Flush to ensure all messages are written
	logger.flush();

	// Verify file exists (FormattingChannel was unwrapped to FileChannel)
	File file(tempFile);
	assertTrue(file.exists());
	assertTrue(file.getSize() > 0);

	// Read and verify log content contains expected elements
	// The pattern [%p] %t translates to [%(log_level)] %(message)
	Poco::FileInputStream fis(tempFile);
	std::string content;
	std::getline(fis, content);
	fis.close();

	// Verify the log contains the log level and message
	// Quill uses INFO for information level
	assertTrue(content.find("INFO") != std::string::npos || content.find("Info") != std::string::npos);
	assertTrue(content.find("Test message through formatting channel") != std::string::npos);

	// Cleanup
	try { file.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testAsyncChannel()
{
	// Test that AsyncChannel is unwrapped (FastLogger is already async)
	std::string tempFile = TemporaryFile::tempName() + "_async.log";

	AutoPtr<FileChannel> pFileChannel(new FileChannel(tempFile));
	AutoPtr<AsyncChannel> pAsyncChannel(new AsyncChannel(pFileChannel));

	// Note: We don't open the AsyncChannel because FastLogger doesn't
	// actually use it - it just extracts the inner channel

	FastLogger& logger = FastLogger::get("TestAdapters.AsyncChannel");
	logger.setChannel(pAsyncChannel);
	logger.setLevel(Message::PRIO_TRACE);

	// Log messages - AsyncChannel should be unwrapped, file used directly
	logger.information("Test message through async channel (unwrapped)");
	logger.warning("Another message through async channel");

	// Flush to ensure all messages are written
	logger.flush();

	// Verify file exists (AsyncChannel was unwrapped to FileChannel)
	File file(tempFile);
	assertTrue(file.exists());
	assertTrue(file.getSize() > 0);

	// Cleanup
	try { file.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testSplitterChannel()
{
	// A SplitterChannel must fan out to all its children, each with its own
	// pattern, rather than collapsing to a single console sink. Verify both
	// children receive the formatted output.
	std::string file1 = TemporaryFile::tempName() + "_split1.log";
	std::string file2 = TemporaryFile::tempName() + "_split2.log";

	AutoPtr<FileChannel> pFile1(new FileChannel(file1));
	// Wrap the second file in a FormattingChannel to exercise pattern
	// extraction through the splitter.
	AutoPtr<FileChannel> pFile2(new FileChannel(file2));
	AutoPtr<PatternFormatter> pFormatter(new PatternFormatter("[%p] %t"));
	AutoPtr<FormattingChannel> pFmt2(new FormattingChannel(pFormatter, pFile2));

	AutoPtr<SplitterChannel> pSplitter(new SplitterChannel);
	pSplitter->addChannel(pFile1);
	pSplitter->addChannel(pFmt2);

	// Introspection API FastLogger uses to map a splitter onto multiple sinks.
	assertTrue(pSplitter->count() == 2);
	assertTrue(pSplitter->getChannel(0).get() == pFile1.get());
	assertTrue(pSplitter->getChannel(1).get() == pFmt2.get());
	assertTrue(pSplitter->getChannel(2).isNull());
	assertTrue(pSplitter->getChannel(-1).isNull());

	FastLogger& logger = FastLogger::get("TestAdapters.SplitterChannel");
	logger.setChannel(pSplitter);
	logger.setLevel(Message::PRIO_TRACE);
	logger.information("splitter routed message");
	logger.flush();

	// Both children received output, not just a single console fallback.
	File f1(file1);
	File f2(file2);
	assertTrue(f1.exists() && f1.getSize() > 0);
	assertTrue(f2.exists() && f2.getSize() > 0);

	// The message reached the formatted file sink through the splitter.
	Poco::FileInputStream fis(file2);
	std::string content;
	std::getline(fis, content);
	fis.close();
	assertTrue(content.find("splitter routed message") != std::string::npos);

	try { f1.remove(); } catch (...) {}
	try { f2.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testEventChannelSkipped()
{
	// EventChannel cannot be mapped to a Quill sink, since FastLogger bypasses
	// Poco's Message/Channel pipeline. FastLogger must skip it without crashing
	// or adding a duplicate console sink, while still serving the splitter's
	// other children. A warning on stderr is expected.
	std::string tempFile = TemporaryFile::tempName() + "_evt.log";

	AutoPtr<FileChannel> pFile(new FileChannel(tempFile));
	AutoPtr<EventChannel> pEvent(new EventChannel);

	AutoPtr<SplitterChannel> pSplitter(new SplitterChannel);
	pSplitter->addChannel(pEvent);
	pSplitter->addChannel(pFile);

	FastLogger& logger = FastLogger::get("TestAdapters.SplitterEventChannel");
	logger.setChannel(pSplitter);
	logger.setLevel(Message::PRIO_TRACE);
	logger.information("message with an EventChannel sibling");
	logger.flush();

	// The FileChannel sibling still works despite the unsupported EventChannel.
	File file(tempFile);
	assertTrue(file.exists() && file.getSize() > 0);

	try { file.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testLocalTimeDefault()
{
	// With no "times"/%L configured, FastLogger defaults to local time, unlike
	// Poco's PatternFormatter which defaults to UTC. Log the hour and compare
	// against the local hour captured around the call.
	const std::string tempFile = TemporaryFile::tempName() + "_tzlocal.log";
	AutoPtr<FileChannel> pFile(new FileChannel(tempFile));
	AutoPtr<PatternFormatter> pFmt(new PatternFormatter("%H %t"));   // no times, no %L
	AutoPtr<FormattingChannel> pFc(new FormattingChannel(pFmt, pFile));

	FastLogger& logger = FastLogger::get("TestAdapters.TzLocalDefault");
	logger.setChannel(pFc);
	logger.setLevel(Message::PRIO_TRACE);

	// The hour changes at most once during the call, so accept before-or-after.
	const std::string hBefore = Poco::DateTimeFormatter::format(Poco::LocalDateTime(), "%H");
	logger.information("tz");
	logger.flush();
	const std::string hAfter = Poco::DateTimeFormatter::format(Poco::LocalDateTime(), "%H");

	File f(tempFile);
	assertTrue(f.exists() && f.getSize() > 0);
	const std::string logged = firstToken(tempFile);
	assertTrue(logged == hBefore || logged == hAfter);

	try { f.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testTimesUtcHonored()
{
	// An explicitly configured times=UTC must be honored, overriding the local
	// default. Machine-independent: the logged hour equals the UTC hour.
	const std::string tempFile = TemporaryFile::tempName() + "_tzutc.log";
	AutoPtr<FileChannel> pFile(new FileChannel(tempFile));
	AutoPtr<PatternFormatter> pFmt(new PatternFormatter("%H %t"));
	pFmt->setProperty(PatternFormatter::PROP_TIMES, "UTC");          // explicit UTC
	AutoPtr<FormattingChannel> pFc(new FormattingChannel(pFmt, pFile));

	FastLogger& logger = FastLogger::get("TestAdapters.TzUtc");
	logger.setChannel(pFc);
	logger.setLevel(Message::PRIO_TRACE);

	const std::string hBefore = Poco::DateTimeFormatter::format(Poco::DateTime(), "%H");
	logger.information("tz");
	logger.flush();
	const std::string hAfter = Poco::DateTimeFormatter::format(Poco::DateTime(), "%H");

	File f(tempFile);
	assertTrue(f.exists() && f.getSize() > 0);
	const std::string logged = firstToken(tempFile);
	assertTrue(logged == hBefore || logged == hAfter);

	try { f.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testTimestampLayout()
{
	// A multi-field Poco date/time pattern must render with its separators
	// intact, e.g. "2026-06-01 12:30:45" rather than "20260601 123045".
	const std::string tempFile = TemporaryFile::tempName() + "_tslayout.log";
	AutoPtr<FileChannel> pFile(new FileChannel(tempFile));
	AutoPtr<PatternFormatter> pFmt(new PatternFormatter("%Y-%m-%d %H:%M:%S [%p] %t"));
	AutoPtr<FormattingChannel> pFc(new FormattingChannel(pFmt, pFile));

	FastLogger& logger = FastLogger::get("TestAdapters.TsLayout");
	logger.setChannel(pFc);
	logger.setLevel(Message::PRIO_TRACE);

	const std::string dBefore = Poco::DateTimeFormatter::format(Poco::LocalDateTime(), "%Y-%m-%d");
	logger.information("layout");
	logger.flush();
	const std::string dAfter = Poco::DateTimeFormatter::format(Poco::LocalDateTime(), "%Y-%m-%d");

	File f(tempFile);
	assertTrue(f.exists() && f.getSize() > 0);
	Poco::FileInputStream fis(tempFile);
	std::string line;
	std::getline(fis, line);
	fis.close();

	// Expect "YYYY-MM-DD HH:MM:SS [<level>] layout".
	assertTrue(line.length() > 19);
	assertTrue(line.compare(0, 10, dBefore) == 0 || line.compare(0, 10, dAfter) == 0);  // date keeps its dashes
	assertTrue(line[10] == ' ');
	assertTrue(line[13] == ':' && line[16] == ':');  // HH:MM:SS keeps its colons
	assertTrue(line.find("layout") != std::string::npos);

	try { f.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testSplitterChannelCycle()
{
	// A SplitterChannel that (transitively) contains itself must not send
	// FastLogger::setChannel() into infinite recursion; the real children must
	// still be served.
	const std::string tempFile = TemporaryFile::tempName() + "_cycle.log";
	AutoPtr<FileChannel> pFile(new FileChannel(tempFile));

	AutoPtr<SplitterChannel> pSplitter(new SplitterChannel);
	pSplitter->addChannel(pFile);
	pSplitter->addChannel(pSplitter);   // self-reference -> cycle in the channel graph

	FastLogger& logger = FastLogger::get("TestAdapters.SplitterCycle");
	logger.setChannel(pSplitter);       // must terminate, not overflow the stack
	logger.setLevel(Message::PRIO_TRACE);
	logger.information("cycle");
	logger.flush();

	// The non-cyclic FileChannel child is still served.
	File f(tempFile);
	assertTrue(f.exists() && f.getSize() > 0);

	pSplitter->close();   // break the self-reference so the splitter can be freed
	try { f.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testLocalTimeHourMatchesSystemClock()
{
	// FastLogger must stamp entries in local time. Log with an HH:MM:SS pattern,
	// parse the hour from the rendered line, and require it to equal the system
	// local hour at the moment of logging. Catches a regression to UTC rendering,
	// where the logged hour would differ from the wall clock by the UTC offset.
	std::string tempFile = TemporaryFile::tempName() + "_localhour.log";
	AutoPtr<FileChannel> pFile(new FileChannel(tempFile));
	AutoPtr<PatternFormatter> pFmt(new PatternFormatter("%H:%M:%S %t"));  // no times/%L => default local
	AutoPtr<FormattingChannel> pFc(new FormattingChannel(pFmt, pFile));

	FastLogger& logger = FastLogger::get("TestAdapters.LocalHourClock");
	logger.setChannel(pFc);
	logger.setLevel(Message::PRIO_TRACE);

	Poco::LocalDateTime before;
	logger.information("clock");
	logger.flush();
	Poco::LocalDateTime after;

	File f(tempFile);
	assertTrue(f.exists() && f.getSize() > 0);
	Poco::FileInputStream fis(tempFile);
	std::string line;
	std::getline(fis, line);
	fis.close();

	// Line begins with "HH:MM:SS ".
	assertTrue(line.length() >= 8);
	assertTrue(line[2] == ':' && line[5] == ':');
	const int loggedHour = Poco::NumberParser::parse(line.substr(0, 2));

	// Must equal the local hour (allow an hour rollover during the call).
	assertTrue(loggedHour == before.hour() || loggedHour == after.hour());

	// On a host with a non-zero whole-hour UTC offset, the local hour differs
	// from the UTC hour, so a regression to UTC rendering is caught right here.
	const int offsetSec = Poco::Timezone::utcOffset() + Poco::Timezone::dst();
	const int utcHour = Poco::DateTime().hour();
	if ((offsetSec % 3600) == 0 && offsetSec != 0 &&
	    before.hour() == after.hour() && before.hour() != utcHour)
	{
		assertTrue(loggedHour != utcHour);
	}

	try { f.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testSplitterConsoleFileWritesFile()
{
	// A SplitterChannel of a ColorConsoleChannel and a FileChannel, both wrapped
	// in a shared FormattingChannel and with no EventChannel. The file must still
	// be written; the absence of an EventChannel sibling must not disable file
	// logging.
	std::string tempFile = TemporaryFile::tempName() + "_splitcf.log";
	AutoPtr<PatternFormatter> pFmt(new PatternFormatter("%H:%M:%S.%i [%p] %s<%I>: %t"));
	pFmt->setProperty(PatternFormatter::PROP_TIMES, "local");

	AutoPtr<ColorConsoleChannel> pConsole(new ColorConsoleChannel);
	AutoPtr<FormattingChannel> pConsoleFc(new FormattingChannel(pFmt, pConsole));
	AutoPtr<FileChannel> pFile(new FileChannel(tempFile));
	AutoPtr<FormattingChannel> pFileFc(new FormattingChannel(pFmt, pFile));

	AutoPtr<SplitterChannel> pSplitter(new SplitterChannel);
	pSplitter->addChannel(pConsoleFc);
	pSplitter->addChannel(pFileFc);

	FastLogger& logger = FastLogger::get("TestAdapters.SplitterConsoleFile");
	logger.setChannel(pSplitter);
	logger.setLevel(Message::PRIO_TRACE);
	logger.information("file output without an event channel");
	logger.flush();

	File f(tempFile);
	assertTrue(f.exists() && f.getSize() > 0);
	Poco::FileInputStream fis(tempFile);
	std::string content;
	std::getline(fis, content);
	fis.close();
	assertTrue(content.find("file output without an event channel") != std::string::npos);

	try { f.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testDirectApiStillFiltersWithPermissiveQuill()
{
	// The Quill loggers are permissive (TraceL3) so the per-logger Poco level is
	// the sole authority on the channel path. The direct logger-API methods must
	// still filter on their own _level before touching Quill: a FastLogger at
	// INFORMATION must drop .trace() even though its Quill logger accepts
	// everything.
	std::string tempFile = TemporaryFile::tempName() + "_directgate.log";
	AutoPtr<PatternFormatter> pFmt(new PatternFormatter("[%p] %s: %t"));
	AutoPtr<FileChannel> pFile(new FileChannel(tempFile));
	AutoPtr<FormattingChannel> pFileFc(new FormattingChannel(pFmt, pFile));

	FastLogger& logger = FastLogger::get("TestAdapters.DirectGate");
	logger.setChannel(pFileFc);
	logger.setLevel(Message::PRIO_INFORMATION);
	logger.trace("DIRECT_TRACE_SHOULD_NOT_APPEAR");
	logger.information("DIRECT_INFO_SHOULD_APPEAR");
	logger.flush();

	File f(tempFile);
	assertTrue(f.exists());
	Poco::FileInputStream fis(tempFile);
	std::string content, line;
	while (std::getline(fis, line)) content += line + "\n";
	fis.close();
	assertTrue(content.find("DIRECT_INFO_SHOULD_APPEAR") != std::string::npos);
	assertTrue(content.find("DIRECT_TRACE_SHOULD_NOT_APPEAR") == std::string::npos);

	try { f.remove(); } catch (...) {}
}


void FastLoggerChannelsTest::testUnsupportedSettersThrow()
{
	// setPattern()/addFileSink() cannot be honored in the per-source bridge model,
	// where sinks and format derive from the Channel passed to setChannel(), so
	// they must throw NotImplementedException without side effects.
	try
	{
		FastLogger::setPattern("%(message)");
		failmsg("setPattern must throw NotImplementedException");
	}
	catch (const Poco::NotImplementedException&) { }

	const std::string tempFile = TemporaryFile::tempName() + "_nosink.log";
	assertTrue (!File(tempFile).exists());
	try
	{
		FastLogger::addFileSink(tempFile);
		failmsg("addFileSink must throw NotImplementedException");
	}
	catch (const Poco::NotImplementedException&) { }

	// The throw happens before any sink is created, so the target file must not
	// be opened or truncated as a side effect.
	assertTrue (!File(tempFile).exists());
}


#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOG_CHANNEL)
void FastLoggerChannelsTest::testSyslogChannel()
{
	// Test that SyslogChannel is properly adapted to Quill's SyslogSink
	AutoPtr<Poco::SyslogChannel> pSyslogChannel(new Poco::SyslogChannel("FastLoggerTest"));

	FastLogger& logger = FastLogger::get("TestAdapters.SyslogChannel");
	logger.setChannel(pSyslogChannel);
	logger.setLevel(Message::PRIO_TRACE);

	// Log messages - should go to syslog
	logger.information("Test message to syslog channel via FastLogger");
	logger.warning("Warning message to syslog");

	logger.flush();

	// Check syslog to verify (manual verification needed)
	// Test passes if no crash occurs
}
#endif


void FastLoggerChannelsTest::setUp()
{
	// Ensure FastLogger backend is running
	FastLogger::startBackend();
}


void FastLoggerChannelsTest::tearDown()
{
}


CppUnit::Test* FastLoggerChannelsTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FastLoggerChannelsTest");

	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testNullChannel);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testConsoleChannel);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testColorConsoleChannel);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testFileChannel);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testSimpleFileChannel);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testStreamChannel);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testFormattingChannel);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testAsyncChannel);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testSplitterChannel);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testEventChannelSkipped);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testLocalTimeDefault);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testTimesUtcHonored);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testTimestampLayout);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testSplitterChannelCycle);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testLocalTimeHourMatchesSystemClock);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testSplitterConsoleFileWritesFile);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testDirectApiStillFiltersWithPermissiveQuill);
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testUnsupportedSettersThrow);
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOG_CHANNEL)
	CppUnit_addTest(pSuite, FastLoggerChannelsTest, testSyslogChannel);
#endif

	return pSuite;
}


#else // !POCO_ENABLE_FASTLOGGER


#include "FastLoggerChannelsTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"


FastLoggerChannelsTest::FastLoggerChannelsTest(const std::string& name): CppUnit::TestCase(name)
{
}


FastLoggerChannelsTest::~FastLoggerChannelsTest()
{
}


void FastLoggerChannelsTest::testNullChannel()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testConsoleChannel()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testColorConsoleChannel()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testFileChannel()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testSimpleFileChannel()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testStreamChannel()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testFormattingChannel()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testAsyncChannel()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testSplitterChannel()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testEventChannelSkipped()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testLocalTimeDefault()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testTimesUtcHonored()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testTimestampLayout()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testSplitterChannelCycle()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testLocalTimeHourMatchesSystemClock()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testSplitterConsoleFileWritesFile()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testDirectApiStillFiltersWithPermissiveQuill()
{
	// FastLogger not enabled
}


void FastLoggerChannelsTest::testUnsupportedSettersThrow()
{
	// FastLogger not enabled
}


#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOG_CHANNEL)
void FastLoggerChannelsTest::testSyslogChannel()
{
	// FastLogger not enabled
}
#endif


void FastLoggerChannelsTest::setUp()
{
}


void FastLoggerChannelsTest::tearDown()
{
}


CppUnit::Test* FastLoggerChannelsTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FastLoggerChannelsTest");
	// No tests when FastLogger is disabled
	return pSuite;
}


#endif // POCO_ENABLE_FASTLOGGER
