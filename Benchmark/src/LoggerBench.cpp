//
// LoggerBench.cpp
//
// Benchmarks for Logger and logging pipeline
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <benchmark/benchmark.h>
#include "Poco/Logger.h"
#include "Poco/Channel.h"
#include "Poco/NullChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Message.h"
#include "Poco/AutoPtr.h"
#include "Poco/Timestamp.h"
#include "Poco/Thread.h"
#include "Poco/Process.h"
#include "Poco/AsyncChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/TemporaryFile.h"
#ifdef POCO_ENABLE_FASTLOGGER
#include "Poco/FastLogger.h"
#endif


using Poco::Logger;
using Poco::Channel;
using Poco::NullChannel;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::Message;
using Poco::AutoPtr;
using Poco::Timestamp;
using Poco::Thread;
using Poco::Process;
using Poco::AsyncChannel;
using Poco::FileChannel;
using Poco::File;
using Poco::Path;
using Poco::TemporaryFile;


namespace {


//
// Level check benchmarks - tests the fast path when logging is disabled
//

static void BM_Logger_LevelCheck_Enabled(benchmark::State& state)
{
	Logger& logger = Logger::get("BenchLogger.LevelCheck.Enabled");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(logger.information());
	}
}
BENCHMARK(BM_Logger_LevelCheck_Enabled);


static void BM_Logger_LevelCheck_Disabled(benchmark::State& state)
{
	Logger& logger = Logger::get("BenchLogger.LevelCheck.Disabled");
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(logger.debug());  // Should be false
	}
}
BENCHMARK(BM_Logger_LevelCheck_Disabled);


//
// Logger::get() - tests logger lookup performance
//

static void BM_Logger_Get_Existing(benchmark::State& state)
{
	// Ensure logger exists first
	Logger::get("BenchLogger.GetExisting");

	for (auto _ : state)
	{
		Logger& logger = Logger::get("BenchLogger.GetExisting");
		benchmark::DoNotOptimize(&logger);
	}
}
BENCHMARK(BM_Logger_Get_Existing);


static void BM_Logger_Get_Hierarchical(benchmark::State& state)
{
	// Create a deep hierarchy
	Logger::get("BenchLogger.Hierarchy.Level1.Level2.Level3");

	for (auto _ : state)
	{
		Logger& logger = Logger::get("BenchLogger.Hierarchy.Level1.Level2.Level3");
		benchmark::DoNotOptimize(&logger);
	}
}
BENCHMARK(BM_Logger_Get_Hierarchical);


//
// Message::init() component benchmarks - measuring individual syscall costs
//

static void BM_Message_Init_ProcessId(benchmark::State& state)
{
	for (auto _ : state)
	{
		long pid = Process::id();
		benchmark::DoNotOptimize(pid);
	}
}
BENCHMARK(BM_Message_Init_ProcessId);


static void BM_Message_Init_CurrentOsTid(benchmark::State& state)
{
	for (auto _ : state)
	{
		long ostid = Thread::currentOsTid();
		benchmark::DoNotOptimize(ostid);
	}
}
BENCHMARK(BM_Message_Init_CurrentOsTid);


static void BM_Message_Init_ThreadCurrent(benchmark::State& state)
{
	for (auto _ : state)
	{
		Thread* pThread = Thread::current();
		benchmark::DoNotOptimize(pThread);
	}
}
BENCHMARK(BM_Message_Init_ThreadCurrent);


static void BM_Message_Init_Timestamp(benchmark::State& state)
{
	for (auto _ : state)
	{
		Timestamp ts;
		benchmark::DoNotOptimize(ts.epochMicroseconds());
	}
}
BENCHMARK(BM_Message_Init_Timestamp);


//
// String copy benchmarks - measuring allocation overhead
//

static void BM_String_Copy_Short(benchmark::State& state)
{
	std::string source = "TestSource";  // 10 chars - within SSO

	for (auto _ : state)
	{
		std::string copy = source;
		benchmark::DoNotOptimize(copy.data());
	}
}
BENCHMARK(BM_String_Copy_Short);


static void BM_String_Copy_Medium(benchmark::State& state)
{
	std::string text = "This is a test log message";  // 26 chars - likely heap

	for (auto _ : state)
	{
		std::string copy = text;
		benchmark::DoNotOptimize(copy.data());
	}
}
BENCHMARK(BM_String_Copy_Medium);


static void BM_String_Copy_Both(benchmark::State& state)
{
	std::string source = "TestSource";
	std::string text = "This is a test log message";

	for (auto _ : state)
	{
		std::string s = source;
		std::string t = text;
		benchmark::DoNotOptimize(s.data());
		benchmark::DoNotOptimize(t.data());
	}
}
BENCHMARK(BM_String_Copy_Both);


//
// Message creation benchmarks
//

static void BM_Message_Create_Copy(benchmark::State& state)
{
	std::string source = "TestSource";
	std::string text = "This is a test log message";

	for (auto _ : state)
	{
		Message msg(source, text, Message::PRIO_INFORMATION);
		benchmark::DoNotOptimize(msg.getText().data());
	}
}
BENCHMARK(BM_Message_Create_Copy);


static void BM_Message_Create_Move(benchmark::State& state)
{
	std::string source = "TestSource";

	for (auto _ : state)
	{
		std::string text = "This is a test log message";
		Message msg(source, std::move(text), Message::PRIO_INFORMATION);
		benchmark::DoNotOptimize(msg.getText().data());
	}
}
BENCHMARK(BM_Message_Create_Move);


static void BM_Message_Create_WithSourceFile(benchmark::State& state)
{
	std::string source = "TestSource";
	std::string text = "This is a test log message";

	for (auto _ : state)
	{
		Message msg(source, text, Message::PRIO_INFORMATION, __FILE__, __LINE__);
		benchmark::DoNotOptimize(msg.getText().data());
	}
}
BENCHMARK(BM_Message_Create_WithSourceFile);


static void BM_Message_Copy(benchmark::State& state)
{
	Message original("TestSource", "This is a test log message", Message::PRIO_INFORMATION, __FILE__, __LINE__);

	for (auto _ : state)
	{
		Message copy(original);
		benchmark::DoNotOptimize(copy.getText().data());
	}
}
BENCHMARK(BM_Message_Copy);


static void BM_Message_Move(benchmark::State& state)
{
	for (auto _ : state)
	{
		state.PauseTiming();
		Message original("TestSource", "This is a test log message", Message::PRIO_INFORMATION, __FILE__, __LINE__);
		state.ResumeTiming();

		Message moved(std::move(original));
		benchmark::DoNotOptimize(moved.getText().data());
	}
}
BENCHMARK(BM_Message_Move);


//
// Logging with NullChannel - measures overhead without I/O
//

static void BM_Logger_Log_NullChannel_Copy(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.NullChannel.Copy");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	std::string msg = "This is a test log message";
	for (auto _ : state)
	{
		logger.information(msg);  // Passes const ref, copies text
	}
}
BENCHMARK(BM_Logger_Log_NullChannel_Copy);


static void BM_Logger_Log_NullChannel_Move(benchmark::State& state)
{
	// This benchmark simulates the case where you have a temporary string
	// from another operation (e.g., format result) that you can move.
	// Both benchmarks must include string creation to be fair.
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.NullChannel.Move");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		std::string msg = "This is a test log message";
		logger.information(std::move(msg));  // Moves text into Message
	}
}
BENCHMARK(BM_Logger_Log_NullChannel_Move);


static void BM_Logger_Log_NullChannel_CopyFresh(benchmark::State& state)
{
	// Fair comparison: create string each iteration, then copy it
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.NullChannel.CopyFresh");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		std::string msg = "This is a test log message";
		logger.information(msg);  // Copies text into Message
	}
}
BENCHMARK(BM_Logger_Log_NullChannel_CopyFresh);


static void BM_Logger_Log_NullChannel_Literal(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.NullChannel.Literal");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");  // String literal
	}
}
BENCHMARK(BM_Logger_Log_NullChannel_Literal);


static void BM_Logger_Log_NullChannel_WithSrcLocation(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.NullChannel.WithFile");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message", __FILE__, __LINE__);
	}
}
BENCHMARK(BM_Logger_Log_NullChannel_WithSrcLocation);


static void BM_Logger_Log_Disabled(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.Disabled");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		// This should short-circuit immediately due to level check
		logger.information("This is a test log message");
	}
}
BENCHMARK(BM_Logger_Log_Disabled);


//
// Logging with FormattingChannel - tests the full pipeline
//

static void BM_Logger_FormattingChannel_Simple(benchmark::State& state)
{
	AutoPtr<NullChannel> pNullChannel(new NullChannel);
	AutoPtr<PatternFormatter> pFormatter(new PatternFormatter("%t"));
	AutoPtr<FormattingChannel> pChannel(new FormattingChannel(pFormatter, pNullChannel));

	Logger& logger = Logger::get("BenchLogger.Formatting.Simple");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}
}
BENCHMARK(BM_Logger_FormattingChannel_Simple);


static void BM_Logger_FormattingChannel_Typical(benchmark::State& state)
{
	AutoPtr<NullChannel> pNullChannel(new NullChannel);
	AutoPtr<PatternFormatter> pFormatter(new PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%p] %s: %t"));
	AutoPtr<FormattingChannel> pChannel(new FormattingChannel(pFormatter, pNullChannel));

	Logger& logger = Logger::get("BenchLogger.Formatting.Typical");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}
}
BENCHMARK(BM_Logger_FormattingChannel_Typical);


static void BM_Logger_FormattingChannel_Full(benchmark::State& state)
{
	AutoPtr<NullChannel> pNullChannel(new NullChannel);
	AutoPtr<PatternFormatter> pFormatter(new PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%p] [%T:%I] %s: %t"));
	AutoPtr<FormattingChannel> pChannel(new FormattingChannel(pFormatter, pNullChannel));

	Logger& logger = Logger::get("BenchLogger.Formatting.Full");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}
}
BENCHMARK(BM_Logger_FormattingChannel_Full);


static void BM_Logger_FormattingChannel_Debug(benchmark::State& state)
{
	AutoPtr<NullChannel> pNullChannel(new NullChannel);
	AutoPtr<PatternFormatter> pFormatter(new PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%p] %s(%U:%u): %t"));
	AutoPtr<FormattingChannel> pChannel(new FormattingChannel(pFormatter, pNullChannel));

	Logger& logger = Logger::get("BenchLogger.Formatting.Debug");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message", __FILE__, __LINE__);
	}
}
BENCHMARK(BM_Logger_FormattingChannel_Debug);


//
// Variadic template logging (format string)
//

static void BM_Logger_Format_OneArg(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.Format.OneArg");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("User %s logged in", std::string("testuser"));
	}
}
BENCHMARK(BM_Logger_Format_OneArg);


static void BM_Logger_Format_ThreeArgs(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.Format.ThreeArgs");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("User %s performed %s on item %d", std::string("testuser"), std::string("update"), 42);
	}
}
BENCHMARK(BM_Logger_Format_ThreeArgs);


static void BM_Logger_Format_Disabled(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.Format.Disabled");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		// Format arguments should NOT be evaluated due to level check
		logger.information("User %s performed %s on item %d", std::string("testuser"), std::string("update"), 42);
	}
}
BENCHMARK(BM_Logger_Format_Disabled);


//
// Logger::format() static method (legacy $0, $1 format)
//

static void BM_Logger_StaticFormat_OneArg(benchmark::State& state)
{
	std::string fmt = "User $0 logged in";
	std::string arg = "testuser";

	for (auto _ : state)
	{
		std::string result = Logger::format(fmt, arg);
		benchmark::DoNotOptimize(result.data());
	}
}
BENCHMARK(BM_Logger_StaticFormat_OneArg);


static void BM_Logger_StaticFormat_FourArgs(benchmark::State& state)
{
	std::string fmt = "User $0 performed $1 on $2 with result $3";

	for (auto _ : state)
	{
		std::string result = Logger::format(fmt, "testuser", "update", "item42", "success");
		benchmark::DoNotOptimize(result.data());
	}
}
BENCHMARK(BM_Logger_StaticFormat_FourArgs);


//
// Hex dump formatting
//

static void BM_Logger_FormatDump_Small(benchmark::State& state)
{
	unsigned char buffer[16];
	for (int i = 0; i < 16; ++i) buffer[i] = i;
	std::string message = "Data dump:";

	for (auto _ : state)
	{
		std::string msg = message;
		Logger::formatDump(msg, buffer, sizeof(buffer));
		benchmark::DoNotOptimize(msg.data());
	}
	state.SetBytesProcessed(state.iterations() * sizeof(buffer));
}
BENCHMARK(BM_Logger_FormatDump_Small);


static void BM_Logger_FormatDump_Medium(benchmark::State& state)
{
	unsigned char buffer[256];
	for (int i = 0; i < 256; ++i) buffer[i] = i;
	std::string message = "Data dump:";

	for (auto _ : state)
	{
		std::string msg = message;
		Logger::formatDump(msg, buffer, sizeof(buffer));
		benchmark::DoNotOptimize(msg.data());
	}
	state.SetBytesProcessed(state.iterations() * sizeof(buffer));
}
BENCHMARK(BM_Logger_FormatDump_Medium);


static void BM_Logger_FormatDump_Large(benchmark::State& state)
{
	unsigned char buffer[4096];
	for (int i = 0; i < 4096; ++i) buffer[i] = i;
	std::string message = "Data dump:";

	for (auto _ : state)
	{
		std::string msg = message;
		Logger::formatDump(msg, buffer, sizeof(buffer));
		benchmark::DoNotOptimize(msg.data());
	}
	state.SetBytesProcessed(state.iterations() * sizeof(buffer));
}
BENCHMARK(BM_Logger_FormatDump_Large);


//
// Macro-based logging (poco_information, etc.)
// These macros include an if-check before constructing the message
//

static void BM_Logger_Macro_Enabled(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.Macro.Enabled");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		poco_information(logger, "This is a test log message");
	}
}
BENCHMARK(BM_Logger_Macro_Enabled);


static void BM_Logger_Macro_Disabled(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.Macro.Disabled");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		// Should short-circuit at the macro level
		poco_information(logger, "This is a test log message");
	}
}
BENCHMARK(BM_Logger_Macro_Disabled);


static void BM_Logger_Macro_Format_Enabled(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.Macro.Format.Enabled");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		poco_information_f(logger, "User %s logged in from %s", std::string("testuser"), std::string("127.0.0.1"));
	}
}
BENCHMARK(BM_Logger_Macro_Format_Enabled);


static void BM_Logger_Macro_Format_Disabled(benchmark::State& state)
{
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.Macro.Format.Disabled");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		// Format should NOT be evaluated due to macro level check
		poco_information_f(logger, "User %s logged in from %s", std::string("testuser"), std::string("127.0.0.1"));
	}
}
BENCHMARK(BM_Logger_Macro_Format_Disabled);


//
// parseLevel - converting string level names to integers
//

static void BM_Logger_ParseLevel(benchmark::State& state)
{
	for (auto _ : state)
	{
		int level = Logger::parseLevel("information");
		benchmark::DoNotOptimize(level);
	}
}
BENCHMARK(BM_Logger_ParseLevel);


//
// Complete pipeline comparison benchmarks
//

static void BM_Logger_Pipeline_DirectLog(benchmark::State& state)
{
	// Direct log without formatting
	AutoPtr<NullChannel> pChannel(new NullChannel);
	Logger& logger = Logger::get("BenchLogger.Pipeline.Direct");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}
}
BENCHMARK(BM_Logger_Pipeline_DirectLog);


static void BM_Logger_Pipeline_WithTypicalFormat(benchmark::State& state)
{
	// Typical production logging pattern with formatting
	AutoPtr<NullChannel> pNullChannel(new NullChannel);
	AutoPtr<PatternFormatter> pFormatter(new PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%p] %s: %t"));
	AutoPtr<FormattingChannel> pChannel(new FormattingChannel(pFormatter, pNullChannel));

	Logger& logger = Logger::get("BenchLogger.Pipeline.TypedFormat");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}
}
BENCHMARK(BM_Logger_Pipeline_WithTypicalFormat);


static void BM_Logger_Pipeline_WithLocalTime(benchmark::State& state)
{
	// Local time conversion adds overhead
	AutoPtr<NullChannel> pNullChannel(new NullChannel);
	AutoPtr<PatternFormatter> pFormatter(new PatternFormatter("%L%Y-%m-%d %H:%M:%S.%i [%p] %s: %t"));
	AutoPtr<FormattingChannel> pChannel(new FormattingChannel(pFormatter, pNullChannel));

	Logger& logger = Logger::get("BenchLogger.Pipeline.LocalTime");
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}
}
BENCHMARK(BM_Logger_Pipeline_WithLocalTime);


//
// AsyncChannel benchmarks - async logging with Poco's AsyncChannel
//

static void BM_Logger_AsyncChannel_NullChannel(benchmark::State& state)
{
	AutoPtr<NullChannel> pNullChannel(new NullChannel);
	AutoPtr<AsyncChannel> pAsyncChannel(new AsyncChannel(pNullChannel));
	pAsyncChannel->open();

	Logger& logger = Logger::get("BenchLogger.Async.NullChannel");
	logger.setChannel(pAsyncChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}

	pAsyncChannel->close();
}
BENCHMARK(BM_Logger_AsyncChannel_NullChannel);


static void BM_Logger_AsyncChannel_WithFormat(benchmark::State& state)
{
	AutoPtr<NullChannel> pNullChannel(new NullChannel);
	AutoPtr<PatternFormatter> pFormatter(new PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%p] %s: %t"));
	AutoPtr<FormattingChannel> pFormattingChannel(new FormattingChannel(pFormatter, pNullChannel));
	AutoPtr<AsyncChannel> pAsyncChannel(new AsyncChannel(pFormattingChannel));
	pAsyncChannel->open();

	Logger& logger = Logger::get("BenchLogger.Async.WithFormat");
	logger.setChannel(pAsyncChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}

	pAsyncChannel->close();
}
BENCHMARK(BM_Logger_AsyncChannel_WithFormat);


static void BM_Logger_AsyncChannel_ToFile(benchmark::State& state)
{
	std::string tempFile = TemporaryFile::tempName() + ".log";

	AutoPtr<FileChannel> pFileChannel(new FileChannel(tempFile));
	AutoPtr<PatternFormatter> pFormatter(new PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%p] %s: %t"));
	AutoPtr<FormattingChannel> pFormattingChannel(new FormattingChannel(pFormatter, pFileChannel));
	AutoPtr<AsyncChannel> pAsyncChannel(new AsyncChannel(pFormattingChannel));
	pAsyncChannel->open();

	Logger& logger = Logger::get("BenchLogger.Async.ToFile");
	logger.setChannel(pAsyncChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}

	pAsyncChannel->close();

	// Cleanup
	try { File(tempFile).remove(); } catch (...) {}
}
BENCHMARK(BM_Logger_AsyncChannel_ToFile);


#ifdef POCO_ENABLE_FASTLOGGER

//
// FastLogger benchmarks - async logging with Quill backend
//
// FastLogger is a high-performance drop-in replacement for Poco::Logger that uses
// the Quill logging library internally. It achieves low latency by:
//   - Using lock-free SPSC (Single-Producer Single-Consumer) queues
//   - Offloading formatting and I/O to a dedicated backend thread
//   - Minimizing work done in the calling thread's hot path
//
// Key metrics to watch:
//   - CPU Time: Time the calling thread spends before returning (the "latency")
//   - Wall Time: Total elapsed time including backend thread work
//
// For async logging, CPU time is the critical metric - it shows how quickly
// control returns to your application. Wall time will be similar between
// AsyncChannel and FastLogger since the total work is roughly the same.
//

// Measures the cost of checking if a log level is enabled (returns true)
static void BM_FastLogger_LevelCheck_Enabled(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.LevelCheck.Enabled");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(logger.information());
	}
}
BENCHMARK(BM_FastLogger_LevelCheck_Enabled);


// Measures the cost of checking if a log level is enabled (returns false)
static void BM_FastLogger_LevelCheck_Disabled(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.LevelCheck.Disabled");
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(logger.debug());  // Should be false
	}
}
BENCHMARK(BM_FastLogger_LevelCheck_Disabled);


// Measures the cost of looking up an existing logger by name
static void BM_FastLogger_Get_Existing(benchmark::State& state)
{
	// Ensure logger exists first
	Poco::FastLogger::get("BenchFastLogger.GetExisting");

	for (auto _ : state)
	{
		Poco::FastLogger& l = Poco::FastLogger::get("BenchFastLogger.GetExisting");
		benchmark::DoNotOptimize(&l);
	}
}
BENCHMARK(BM_FastLogger_Get_Existing);


// Measures logging overhead with null sink (no I/O)
// This isolates the pure queueing cost
static void BM_FastLogger_Log_NullChannel(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.NullChannel");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}
}
BENCHMARK(BM_FastLogger_Log_NullChannel);


// Measures the cost when logging is disabled by level
// Should be near-zero as the call short-circuits before queueing
static void BM_FastLogger_Log_Disabled(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.Disabled");
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		// This should short-circuit immediately due to level check
		logger.information("This is a test log message");
	}
}
BENCHMARK(BM_FastLogger_Log_Disabled);


// Measures logging a string literal (no dynamic allocation)
static void BM_FastLogger_Log_Literal(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.Literal");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");  // String literal
	}
}
BENCHMARK(BM_FastLogger_Log_Literal);


// Measures logging with one format argument
// Includes Poco::format() overhead in the calling thread
static void BM_FastLogger_Format_OneArg(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.Format.OneArg");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("User %s logged in", std::string("testuser"));
	}
}
BENCHMARK(BM_FastLogger_Format_OneArg);


// Measures logging with three format arguments
// More format args = more work in calling thread
static void BM_FastLogger_Format_ThreeArgs(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.Format.ThreeArgs");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("User %s performed %s on item %d", std::string("testuser"), std::string("update"), 42);
	}
}
BENCHMARK(BM_FastLogger_Format_ThreeArgs);


// Verifies that format arguments are NOT evaluated when level is disabled
// Should be near-zero if level check happens before format evaluation
static void BM_FastLogger_Format_Disabled(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.Format.Disabled");
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		// Format arguments should NOT be evaluated due to level check
		logger.information("User %s performed %s on item %d", std::string("testuser"), std::string("update"), 42);
	}
}
BENCHMARK(BM_FastLogger_Format_Disabled);


// Measures logging to an actual file
// Note: Setup overhead is included; use fixture version for accurate comparison
static void BM_FastLogger_ToFile(benchmark::State& state)
{
	std::string tempFile = TemporaryFile::tempName() + ".log";

	AutoPtr<FileChannel> pFileChannel(new FileChannel(tempFile));
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.ToFile");
	logger.setChannel(pFileChannel);  // Override null sink with file
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}

	try { File(tempFile).remove(); } catch (...) {}
}
BENCHMARK(BM_FastLogger_ToFile);


//
// Direct comparison: Logger+AsyncChannel vs FastLogger
//
// These benchmarks compare the two async logging approaches writing to files.
// Using fixtures ensures setup (file opening, channel creation) happens once
// before all iterations, not per-iteration.
//
// Expected results:
//   - Wall time: Similar (both do the same total work)
//   - CPU time: FastLogger should be 20-60x faster (less work in hot path)
//

class AsyncChannelFileFixture : public benchmark::Fixture
{
public:
	void SetUp(benchmark::State&) override
	{
		tempFile = TemporaryFile::tempName() + "_async.log";
		pFileChannel = new FileChannel(tempFile);
		pFormatter = new PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%p] %s: %t");
		pFormattingChannel = new FormattingChannel(pFormatter, pFileChannel);
		pAsyncChannel = new AsyncChannel(pFormattingChannel);
		pAsyncChannel->open();

		pLogger = &Logger::get("BenchComparison.Async.File");
		pLogger->setChannel(pAsyncChannel);
		pLogger->setLevel(Message::PRIO_TRACE);
	}

	void TearDown(benchmark::State&) override
	{
		pAsyncChannel->close();
		try { File(tempFile).remove(); } catch (...) {}
	}

	std::string tempFile;
	AutoPtr<FileChannel> pFileChannel;
	AutoPtr<PatternFormatter> pFormatter;
	AutoPtr<FormattingChannel> pFormattingChannel;
	AutoPtr<AsyncChannel> pAsyncChannel;
	Logger* pLogger = nullptr;
};

// Logger + AsyncChannel writing to file
// Uses mutex-based queue internally
BENCHMARK_F(AsyncChannelFileFixture, BM_Comparison_Logger_AsyncChannel_File)(benchmark::State& state)
{
	for (auto _ : state)
	{
		pLogger->information("This is a test log message for comparison");
	}
}


class FastLoggerFileFixture : public benchmark::Fixture
{
public:
	void SetUp(benchmark::State&) override
	{
		tempFile = TemporaryFile::tempName() + "_fast.log";
		pFileChannel = new FileChannel(tempFile);
		pLogger = &Poco::FastLogger::get("BenchComparison.Fast.File");
		pLogger->setChannel(pFileChannel);
		pLogger->setLevel(Message::PRIO_TRACE);
	}

	void TearDown(benchmark::State&) override
	{
		try { File(tempFile).remove(); } catch (...) {}
	}

	std::string tempFile;
	AutoPtr<FileChannel> pFileChannel;
	Poco::FastLogger* pLogger = nullptr;
};

// FastLogger writing to file
// Uses lock-free SPSC queue internally (Quill)
BENCHMARK_F(FastLoggerFileFixture, BM_Comparison_FastLogger_File)(benchmark::State& state)
{
	for (auto _ : state)
	{
		pLogger->information("This is a test log message for comparison");
	}
}


//
// CPU Affinity comparison: Both loggers with affinity enabled
// This provides a fair comparison when both have their backend threads pinned
//

class AsyncChannelFileAffinityFixture : public benchmark::Fixture
{
public:
	void SetUp(benchmark::State&) override
	{
		tempFile = TemporaryFile::tempName() + "_async_affinity.log";
		pFileChannel = new FileChannel(tempFile);
		pFormatter = new PatternFormatter("%Y-%m-%d %H:%M:%S.%i [%p] %s: %t");
		pFormattingChannel = new FormattingChannel(pFormatter, pFileChannel);
		pAsyncChannel = new AsyncChannel(pFormattingChannel);
		pAsyncChannel->setProperty("enableCpuAffinity", "true");
		pAsyncChannel->open();

		pLogger = &Logger::get("BenchComparison.Async.File.Affinity");
		pLogger->setChannel(pAsyncChannel);
		pLogger->setLevel(Message::PRIO_TRACE);
	}

	void TearDown(benchmark::State&) override
	{
		pAsyncChannel->close();
		try { File(tempFile).remove(); } catch (...) {}
	}

	std::string tempFile;
	AutoPtr<FileChannel> pFileChannel;
	AutoPtr<PatternFormatter> pFormatter;
	AutoPtr<FormattingChannel> pFormattingChannel;
	AutoPtr<AsyncChannel> pAsyncChannel;
	Logger* pLogger = nullptr;
};

// Logger + AsyncChannel with CPU affinity enabled
BENCHMARK_F(AsyncChannelFileAffinityFixture, BM_Comparison_Logger_AsyncChannel_File_Affinity)(benchmark::State& state)
{
	for (auto _ : state)
	{
		pLogger->information("This is a test log message for comparison");
	}
}


// Note: FastLogger backend options (including enableCpuAffinity) can only be set
// before the backend starts. Since all FastLogger benchmarks share the same backend,
// we cannot have separate affinity/non-affinity FastLogger benchmarks in the same run.
// The FastLoggerFileFixture benchmark above uses the default (no affinity) configuration.
// To benchmark FastLogger with affinity, call FastLogger::setBackendOption("enableCpuAffinity", "true")
// before creating any loggers.


//
// Throughput comparison with varying message sizes (null sink)
//
// These tests isolate queue/threading overhead by discarding output.
// Short messages test minimum overhead; long messages test memory/copy costs.
//

// AsyncChannel with short message (~9 bytes)
static void BM_Comparison_AsyncChannel_ShortMsg(benchmark::State& state)
{
	AutoPtr<NullChannel> pNullChannel(new NullChannel);
	AutoPtr<AsyncChannel> pAsyncChannel(new AsyncChannel(pNullChannel));
	pAsyncChannel->open();

	Logger& logger = Logger::get("BenchComparison.Async.Short");
	logger.setChannel(pAsyncChannel);
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("Short msg");
	}

	pAsyncChannel->close();
}
BENCHMARK(BM_Comparison_AsyncChannel_ShortMsg);


// FastLogger with short message (~9 bytes)
static void BM_Comparison_FastLogger_ShortMsg(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchComparison.Fast.Short");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("Short msg");
	}
}
BENCHMARK(BM_Comparison_FastLogger_ShortMsg);


// AsyncChannel with long message (256 bytes)
// Tests how message size affects queueing overhead
static void BM_Comparison_AsyncChannel_LongMsg(benchmark::State& state)
{
	AutoPtr<NullChannel> pNullChannel(new NullChannel);
	AutoPtr<AsyncChannel> pAsyncChannel(new AsyncChannel(pNullChannel));
	pAsyncChannel->open();

	Logger& logger = Logger::get("BenchComparison.Async.Long");
	logger.setChannel(pAsyncChannel);
	logger.setLevel(Message::PRIO_TRACE);

	std::string longMsg(256, 'x');
	for (auto _ : state)
	{
		logger.information(longMsg);
	}

	pAsyncChannel->close();
}
BENCHMARK(BM_Comparison_AsyncChannel_LongMsg);


// FastLogger with long message (256 bytes)
// Tests how message size affects queueing overhead
static void BM_Comparison_FastLogger_LongMsg(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchComparison.Fast.Long");
	logger.setLevel(Message::PRIO_TRACE);

	std::string longMsg(256, 'x');
	for (auto _ : state)
	{
		logger.information(longMsg);
	}
}
BENCHMARK(BM_Comparison_FastLogger_LongMsg);


#endif // POCO_ENABLE_FASTLOGGER


} // namespace
