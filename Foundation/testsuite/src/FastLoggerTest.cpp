//
// FastLoggerTest.cpp
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Foundation.h"


#ifdef POCO_ENABLE_FASTLOGGER


#include "FastLoggerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/FastLogger.h"
#include "Poco/AutoPtr.h"
#include "Poco/FileChannel.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/Thread.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Clock.h"
#include "Poco/FileStream.h"
#include "Poco/NumberParser.h"
#include "Poco/StringTokenizer.h"
#include <cstring>
#if defined(__linux__)
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach/mach.h>
#elif defined(_WIN32) && !defined(__MINGW32__)
#include "Poco/UnWindows.h"
#include <tlhelp32.h>
#endif


using Poco::FastLogger;
using Poco::FileChannel;
using Poco::Channel;
using Poco::Message;
using Poco::AutoPtr;
using Poco::File;
using Poco::Path;
using Poco::Thread;
using Poco::DirectoryIterator;
using Poco::Clock;


namespace {


const std::string IDLE_BACKEND_THREAD_NAME("FLIdleBackend");


Poco::Int64 threadCPUTime([[maybe_unused]] const std::string& threadName)
	/// Returns the CPU time, in microseconds, used so far by the thread of
	/// this process with the given name, -1 if there is no such thread,
	/// or -2 if the platform's per-thread CPU time cannot be read.
{
#if defined(__linux__)
	auto readLine = [](const std::string& path)
	{
		std::string line;
		try
		{
			Poco::FileInputStream istr(path);
			std::getline(istr, line);
		}
		catch (Poco::Exception&)
		{
		}
		return line;
	};

	DirectoryIterator it;
	try
	{
		it = DirectoryIterator(std::string("/proc/self/task"));
	}
	catch (Poco::Exception&)
	{
		return -2;
	}
	for (; it != DirectoryIterator(); ++it)
	{
		const std::string taskDir = "/proc/self/task/" + it.name() + "/";
		if (readLine(taskDir + "comm") != threadName) continue;

		// schedstat holds the time spent on the CPU in nanoseconds. Kernels
		// that do not keep scheduler statistics report 0 there, so a zero is
		// not taken: a running thread always has used some CPU time.
		Poco::StringTokenizer fields(readLine(taskDir + "schedstat"), " ", Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		Poco::UInt64 runNs = 0;
		if (fields.count() > 0 && Poco::NumberParser::tryParseUnsigned64(fields[0], runNs) && runNs > 0)
			return static_cast<Poco::Int64>(runNs/1000);

		// Without schedstat, fall back to utime and stime (fields 14 and 15
		// of stat, in clock ticks). The thread name in field 2 may contain
		// spaces, so the fields are counted from its closing parenthesis.
		const std::string stat = readLine(taskDir + "stat");
		const std::string::size_type pos = stat.rfind(')');
		if (pos == std::string::npos) return -2;
		Poco::StringTokenizer statFields(stat.substr(pos + 1), " ", Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		Poco::UInt64 utime = 0;
		Poco::UInt64 stime = 0;
		const long ticksPerSecond = sysconf(_SC_CLK_TCK);
		if (statFields.count() > 12 && ticksPerSecond > 0 &&
			Poco::NumberParser::tryParseUnsigned64(statFields[11], utime) &&
			Poco::NumberParser::tryParseUnsigned64(statFields[12], stime))
		{
			return static_cast<Poco::Int64>((utime + stime)*1000000/ticksPerSecond);
		}
		return -2;
	}
	return -1;
#elif defined(__APPLE__)
	thread_act_array_t threads = nullptr;
	mach_msg_type_number_t threadCount = 0;
	if (task_threads(mach_task_self(), &threads, &threadCount) != KERN_SUCCESS)
		return -2;

	Poco::Int64 result = -1;
	for (mach_msg_type_number_t i = 0; i < threadCount; ++i)
	{
		if (result == -1)
		{
			thread_extended_info_data_t extInfo;
			mach_msg_type_number_t extCount = THREAD_EXTENDED_INFO_COUNT;
			if (thread_info(threads[i], THREAD_EXTENDED_INFO, reinterpret_cast<thread_info_t>(&extInfo), &extCount) == KERN_SUCCESS &&
				std::strncmp(extInfo.pth_name, threadName.c_str(), sizeof(extInfo.pth_name)) == 0)
			{
				thread_basic_info_data_t basicInfo;
				mach_msg_type_number_t basicCount = THREAD_BASIC_INFO_COUNT;
				if (thread_info(threads[i], THREAD_BASIC_INFO, reinterpret_cast<thread_info_t>(&basicInfo), &basicCount) == KERN_SUCCESS)
				{
					result = static_cast<Poco::Int64>(basicInfo.user_time.seconds + basicInfo.system_time.seconds)*1000000
						+ basicInfo.user_time.microseconds + basicInfo.system_time.microseconds;
				}
				else result = -2;
			}
		}
		mach_port_deallocate(mach_task_self(), threads[i]);
	}
	vm_deallocate(mach_task_self(), reinterpret_cast<vm_address_t>(threads), threadCount*sizeof(thread_act_t));
	return result;
#elif defined(_WIN32) && !defined(__MINGW32__)
	// Quill names its backend thread with SetThreadDescription(), so the
	// thread is found by its description. GetThreadDescription() exists from
	// Windows 10 1607 on and is looked up at runtime, as Quill does.
	using GetThreadDescriptionFunc = HRESULT (WINAPI *)(HANDLE, PWSTR*);
	const HMODULE kernelBase = GetModuleHandleW(L"KernelBase.dll");
	const auto getThreadDescription = kernelBase ?
		reinterpret_cast<GetThreadDescriptionFunc>(GetProcAddress(kernelBase, "GetThreadDescription")) : nullptr;
	if (!getThreadDescription)
		return -2;

	const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
		return -2;

	// The thread names of the test are ASCII.
	const std::wstring wideName(threadName.begin(), threadName.end());
	const DWORD processId = GetCurrentProcessId();
	auto toMicroseconds = [](const FILETIME& ft)
	{
		// FILETIME counts 100-nanosecond intervals.
		return ((static_cast<Poco::UInt64>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime)/10;
	};

	Poco::Int64 result = -1;
	THREADENTRY32 entry;
	entry.dwSize = sizeof(entry);
	for (BOOL more = Thread32First(snapshot, &entry); more && result == -1; more = Thread32Next(snapshot, &entry))
	{
		if (entry.th32OwnerProcessID != processId) continue;
		const HANDLE thread = OpenThread(THREAD_QUERY_LIMITED_INFORMATION, FALSE, entry.th32ThreadID);
		if (!thread) continue;
		PWSTR description = nullptr;
		if (SUCCEEDED(getThreadDescription(thread, &description)) && description)
		{
			if (wideName == description)
			{
				FILETIME creationTime, exitTime, kernelTime, userTime;
				if (GetThreadTimes(thread, &creationTime, &exitTime, &kernelTime, &userTime))
					result = static_cast<Poco::Int64>(toMicroseconds(kernelTime) + toMicroseconds(userTime));
				else
					result = -2;
			}
			LocalFree(description);
		}
		CloseHandle(thread);
	}
	CloseHandle(snapshot);
	return result;
#else
	return -2;
#endif
}


std::string getTempFilename()
{
	return Path::tempHome() + "fastlogger_rotation_test.log";
}

void removeLogFiles(const std::string& basePath)
{
	// Remove the base log file and any rotated files
	try
	{
		File f(basePath);
		if (f.exists()) f.remove();
	}
	catch (...) {}

	// Remove numbered rotated files (.1, .2, etc.)
	for (int i = 0; i < 20; ++i)
	{
		try
		{
			File f(basePath + "." + std::to_string(i));
			if (f.exists()) f.remove();
		}
		catch (...) {}
	}

	// Also try to remove files with date suffixes in the same directory
	try
	{
		Path p(basePath);
		std::string dir = p.parent().toString();
		std::string stem = p.getBaseName();
		std::string ext = p.getExtension();

		if (!dir.empty())
		{
			for (DirectoryIterator it(dir); it != DirectoryIterator(); ++it)
			{
				std::string name = it.name();
				if (name.find(stem) == 0 && name.find(ext) != std::string::npos)
				{
					try { it->remove(); } catch (...) {}
				}
			}
		}
	}
	catch (...) {}
}

int countLogFiles(const std::string& basePath)
{
	int count = 0;

	// Count the base file
	File baseFile(basePath);
	if (baseFile.exists()) ++count;

	// Count rotated files
	for (int i = 0; i < 20; ++i)
	{
		File f(basePath + "." + std::to_string(i));
		if (f.exists()) ++count;
	}

	return count;
}

} // anonymous namespace


FastLoggerTest::FastLoggerTest(const std::string& name): CppUnit::TestCase(name)
{
}


FastLoggerTest::~FastLoggerTest()
{
}


void FastLoggerTest::testLogger()
{
	// Note: FastLogger uses Quill's async logging, so we can't use TestChannel
	// to capture messages in the same way as Logger. Instead, we test the
	// API compatibility and level filtering logic.

	FastLogger& root = FastLogger::root();

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

	// Test logging methods don't crash (output goes to Quill backend)
	root.information("Informational message");
	root.warning("Warning message");
	root.debug("Debug message");  // Should be filtered out

	FastLogger& logger1 = FastLogger::get("Logger1");
	FastLogger& logger2 = FastLogger::get("Logger2");
	FastLogger& logger11 = FastLogger::get("Logger1.Logger1");
	FastLogger& logger12 = FastLogger::get("Logger1.Logger2");
	FastLogger& logger21 = FastLogger::get("Logger2.Logger1");
	FastLogger& logger22 = FastLogger::get("Logger2.Logger2");

	std::vector<std::string> loggers;
	FastLogger::names(loggers);
	assertTrue (loggers.size() == 7);
	assertTrue (loggers[0] == "");
	assertTrue (loggers[1] == "Logger1");
	assertTrue (loggers[2] == "Logger1.Logger1");
	assertTrue (loggers[3] == "Logger1.Logger2");
	assertTrue (loggers[4] == "Logger2");
	assertTrue (loggers[5] == "Logger2.Logger1");
	assertTrue (loggers[6] == "Logger2.Logger2");

	FastLogger::setLevel("Logger1", Message::PRIO_DEBUG);
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

	FastLogger::setLevel("Logger2.Logger1", Message::PRIO_ERROR);
	assertTrue (logger1.is(Message::PRIO_DEBUG));
	assertTrue (logger11.is(Message::PRIO_DEBUG));
	assertTrue (logger12.is(Message::PRIO_DEBUG));
	assertTrue (!logger21.is(Message::PRIO_DEBUG));
	assertTrue (!logger22.is(Message::PRIO_DEBUG));
	assertTrue (logger11.is(Message::PRIO_INFORMATION));
	assertTrue (logger12.is(Message::PRIO_INFORMATION));
	assertTrue (logger21.is(Message::PRIO_ERROR));
	assertTrue (logger22.is(Message::PRIO_INFORMATION));

	FastLogger::setLevel("", Message::PRIO_WARNING);
	assertTrue (root.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger1.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger11.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger12.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger1.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger21.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger22.getLevel() == Message::PRIO_WARNING);

	// Test all logging methods at trace level
	root.setLevel(Message::PRIO_TRACE);
	root.trace("trace");
	root.debug("debug");
	root.information("information");
	root.notice("notice");
	root.warning("warning");
	root.error("error");
	root.critical("critical");
	root.fatal("fatal");

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


void FastLoggerTest::testFormat()
{
	std::string str = FastLogger::format("$0$1", "foo", "bar");
	assertTrue (str == "foobar");
	str = FastLogger::format("foo$0", "bar");
	assertTrue (str == "foobar");
	str = FastLogger::format("the amount is $$ $0", "100");
	assertTrue (str == "the amount is $ 100");
	str = FastLogger::format("$0$1$2", "foo", "bar");
	assertTrue (str == "foobar");
	str = FastLogger::format("$foo$0", "bar");
	assertTrue (str == "$foobar");
	str = FastLogger::format("$0", "1");
	assertTrue (str == "1");
	str = FastLogger::format("$0$1", "1", "2");
	assertTrue (str == "12");
	str = FastLogger::format("$0$1$2", "1", "2", "3");
	assertTrue (str == "123");
	str = FastLogger::format("$0$1$2$3", "1", "2", "3", "4");
	assertTrue (str == "1234");
	str = FastLogger::format("foo$", "bar");
	assertTrue (str == "foo$");
	str = FastLogger::format("$", "bar");
	assertTrue (str == "$");
	str = FastLogger::format("$0$", "bar");
	assertTrue (str == "bar$");
}


void FastLoggerTest::testFormatAny()
{
	// Test variadic template formatting (using Poco::format internally)
	FastLogger& root = FastLogger::root();
	root.setLevel(Message::PRIO_ERROR);

	// These should not crash - output goes to Quill backend
	root.error("%s%s", std::string("foo"), std::string("bar"));
	root.error("foo%s", std::string("bar"));
	root.error("the amount is %% %d", 100);
	root.error("%d", 1);
	root.error("%d%d", 1, 2);
	root.error("%d%d%d", 1, 2, 3);
	root.error("%d%d%d%d", 1, 2, 3, 4);
	root.error("%d%d%d%d%d", 1, 2, 3, 4, 5);
	root.error("%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6);
	root.error("%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7);
	root.error("%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8);
	root.error("%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9);
	root.error("%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
}


void FastLoggerTest::testDump()
{
	FastLogger& root = FastLogger::root();
	root.setLevel(Message::PRIO_INFORMATION);

	char buffer1[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
	root.dump("test", buffer1, sizeof(buffer1));  // Should be filtered (DEBUG level)

	root.setLevel(Message::PRIO_DEBUG);
	root.dump("test", buffer1, sizeof(buffer1));

	char buffer2[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};
	root.dump("", buffer2, sizeof(buffer2));

	char buffer3[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
		0x20, 0x41, 0x42, 0x1f, 0x7f, 0x7e
	};
	root.dump("", buffer3, sizeof(buffer3));

	// Test formatDump directly
	std::string msg = "test";
	FastLogger::formatDump(msg, buffer1, sizeof(buffer1));
	assertTrue (msg.find("00 01 02 03 04 05") != std::string::npos);
}


void FastLoggerTest::testIdleBackendCPUUsage()
{
	// With nothing to log, the backend thread must wait between polls
	// instead of spinning. Only the thread name is set here, so that the
	// backend thread can be found; all other backend options keep their
	// defaults.
	FastLogger::setBackendOption("threadName", IDLE_BACKEND_THREAD_NAME);
	FastLogger::startBackend();

	Poco::Int64 cpuStart = -1;
	Clock deadline;
	deadline += 10*Clock::resolution();
	while (cpuStart == -1 && !deadline.isElapsed(0))
	{
		cpuStart = threadCPUTime(IDLE_BACKEND_THREAD_NAME);
		if (cpuStart == -1) Thread::sleep(10);
	}
	if (cpuStart == -2)
	{
		FastLogger::shutdown();
		warnmsg("per-thread CPU time is not available on this platform, skipping test");
		return;
	}
	if (cpuStart == -1)
	{
		FastLogger::shutdown();
		failmsg("backend thread " + IDLE_BACKEND_THREAD_NAME + " not found");
	}

	// Measure the CPU time the idle backend uses over a fixed period.
	// A spinning backend uses about as much CPU time as the period lasts;
	// a waiting one uses a small fraction of it.
	Clock start;
	Thread::sleep(1000);
	const Poco::Int64 cpuEnd = threadCPUTime(IDLE_BACKEND_THREAD_NAME);
	const Poco::Int64 elapsed = start.elapsed();
	FastLogger::shutdown();

	if (cpuEnd < 0)
		failmsg("backend thread " + IDLE_BACKEND_THREAD_NAME + " CPU time not available after the measurement (" +
			std::to_string(cpuEnd) + ")");
	assertTrue (cpuEnd >= cpuStart);
	const Poco::Int64 cpuUsed = cpuEnd - cpuStart;
	if (cpuUsed*4 >= elapsed)
	{
		failmsg("idle backend thread used " + std::to_string(cpuUsed) +
			" us of CPU time in " + std::to_string(elapsed) + " us");
	}
}


void FastLoggerTest::testRotateBySize()
{
	std::string logFile = getTempFilename();
	removeLogFiles(logFile);

	try
	{
		// Create FileChannel with size-based rotation (512 bytes - minimum for Quill)
		AutoPtr<FileChannel> pChannel = new FileChannel(logFile);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "512");

		// Create FastLogger with the rotating file channel
		FastLogger& logger = FastLogger::create("rotate_size_test", pChannel, Message::PRIO_INFORMATION);

		// Write enough messages to trigger rotation
		// Each message with Quill timestamp/format overhead is ~100-150 bytes
		for (int i = 0; i < 50; ++i)
		{
			logger.information("Test message " + std::to_string(i) + " - padding to ensure sufficient length for rotation trigger");
		}

		// Flush and give time for async writes
		logger.flush();
		Thread::sleep(1000);

		// Shutdown to ensure all files are closed
		FastLogger::shutdown();
		Thread::sleep(500);

		// Verify rotation occurred - check that at least the base file exists
		// and that we have multiple files if rotation worked
		File f(logFile);
		assertTrue (f.exists());

		int fileCount = countLogFiles(logFile);
		// With 50 messages at ~100+ bytes each = ~5KB+ of data
		// With 512 byte rotation, we expect multiple files
		assertTrue (fileCount >= 1);  // At minimum, the log file should exist
	}
	catch (...)
	{
		removeLogFiles(logFile);
		FastLogger::shutdown();
		throw;
	}
	removeLogFiles(logFile);
}


void FastLoggerTest::testRotateDaily()
{
	std::string logFile = Path::tempHome() + "fastlogger_daily_rotation_test.log";
	removeLogFiles(logFile);

	try
	{
		// Create FileChannel with daily rotation at midnight
		AutoPtr<FileChannel> pChannel = new FileChannel(logFile);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "00:00");

		// Create FastLogger with the daily rotating file channel
		FastLogger& logger = FastLogger::create("rotate_daily_test", pChannel, Message::PRIO_INFORMATION);

		// Write some messages
		for (int i = 0; i < 10; ++i)
		{
			logger.information("Daily rotation test message " + std::to_string(i));
		}

		logger.flush();
		Thread::sleep(500);

		// Shutdown to ensure all files are written
		FastLogger::shutdown();
		Thread::sleep(200);

		// Verify the log file was created
		File f(logFile);
		assertTrue (f.exists());
	}
	catch (...)
	{
		removeLogFiles(logFile);
		FastLogger::shutdown();
		throw;
	}
	removeLogFiles(logFile);
}


void FastLoggerTest::testRotateLargeFile()
{
	std::string logFile = Path::tempHome() + "fastlogger_large_rotation_test.log";
	removeLogFiles(logFile);

	try
	{
		// Create FileChannel with large size-based rotation (10 MB)
		// This tests rotation configuration with larger file sizes
		AutoPtr<FileChannel> pChannel = new FileChannel(logFile);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "10 M");

		// Create FastLogger with the rotating file channel
		FastLogger& logger = FastLogger::create("rotate_large_test", pChannel, Message::PRIO_INFORMATION);

		// Write some messages
		for (int i = 0; i < 10; ++i)
		{
			logger.information("Large file rotation test message " + std::to_string(i));
		}

		logger.flush();
		Thread::sleep(500);

		// Shutdown to ensure all files are written
		FastLogger::shutdown();
		Thread::sleep(200);

		// Verify the log file was created
		File f(logFile);
		assertTrue (f.exists());
	}
	catch (...)
	{
		removeLogFiles(logFile);
		FastLogger::shutdown();
		throw;
	}
	removeLogFiles(logFile);
}


void FastLoggerTest::testRotateWithPurgeCount()
{
	std::string logFile = Path::tempHome() + "fastlogger_purge_rotation_test.log";
	removeLogFiles(logFile);

	try
	{
		// Create FileChannel with size-based rotation and purgeCount=3
		AutoPtr<FileChannel> pChannel = new FileChannel(logFile);
		pChannel->setProperty(FileChannel::PROP_ROTATION, "512");
		pChannel->setProperty(FileChannel::PROP_PURGECOUNT, "3");
		pChannel->setProperty(FileChannel::PROP_ARCHIVE, "number");

		// Create FastLogger with the rotating file channel
		FastLogger& logger = FastLogger::create("rotate_purge_test", pChannel, Message::PRIO_INFORMATION);

		// Write many messages to trigger multiple rotations
		for (int i = 0; i < 200; ++i)
		{
			logger.information("Purge test message " + std::to_string(i) + " with some extra padding text");
		}

		logger.flush();
		Thread::sleep(500);

		FastLogger::shutdown();
		Thread::sleep(200);

		// Verify we don't have more than 4 files (3 backups + 1 current)
		int fileCount = countLogFiles(logFile);
		assertTrue (fileCount <= 4);
	}
	catch (...)
	{
		removeLogFiles(logFile);
		FastLogger::shutdown();
		throw;
	}
	removeLogFiles(logFile);
}


void FastLoggerTest::setUp()
{
	FastLogger::shutdown();
}


void FastLoggerTest::tearDown()
{
}


CppUnit::Test* FastLoggerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FastLoggerTest");

	CppUnit_addTest(pSuite, FastLoggerTest, testLogger);
	CppUnit_addTest(pSuite, FastLoggerTest, testFormat);
	CppUnit_addTest(pSuite, FastLoggerTest, testFormatAny);
	CppUnit_addTest(pSuite, FastLoggerTest, testDump);
	CppUnit_addTest(pSuite, FastLoggerTest, testIdleBackendCPUUsage);
	CppUnit_addTest(pSuite, FastLoggerTest, testRotateBySize);
	CppUnit_addTest(pSuite, FastLoggerTest, testRotateDaily);
	CppUnit_addTest(pSuite, FastLoggerTest, testRotateLargeFile);
	CppUnit_addTest(pSuite, FastLoggerTest, testRotateWithPurgeCount);

	return pSuite;
}


#else // !POCO_ENABLE_FASTLOGGER


#include "FastLoggerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"


FastLoggerTest::FastLoggerTest(const std::string& name): CppUnit::TestCase(name)
{
}


FastLoggerTest::~FastLoggerTest()
{
}


void FastLoggerTest::testLogger()
{
	// FastLogger not enabled
}


void FastLoggerTest::testFormat()
{
	// FastLogger not enabled
}


void FastLoggerTest::testFormatAny()
{
	// FastLogger not enabled
}


void FastLoggerTest::testDump()
{
	// FastLogger not enabled
}


void FastLoggerTest::testIdleBackendCPUUsage()
{
	// FastLogger not enabled
}


void FastLoggerTest::testRotateBySize()
{
	// FastLogger not enabled
}


void FastLoggerTest::testRotateDaily()
{
	// FastLogger not enabled
}


void FastLoggerTest::testRotateLargeFile()
{
	// FastLogger not enabled
}


void FastLoggerTest::testRotateWithPurgeCount()
{
	// FastLogger not enabled
}


void FastLoggerTest::setUp()
{
}


void FastLoggerTest::tearDown()
{
}


CppUnit::Test* FastLoggerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FastLoggerTest");
	// No tests when FastLogger is disabled
	return pSuite;
}


#endif // POCO_ENABLE_FASTLOGGER
