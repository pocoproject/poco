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
#include "Poco/Format.h"
#include "Poco/Util/LoggingConfigurator.h"
#include "Poco/FastLogger.h"


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
using Poco::Util::LoggingConfigurator;


namespace {


//
// Level check benchmarks - tests the fast path when logging is disabled
//

static void Logger_LevelCheck_Enabled(benchmark::State& state)
{
	Logger& logger = Logger::get("BenchLogger.LevelCheck.Enabled");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(logger.information());
	}
}
BENCHMARK(Logger_LevelCheck_Enabled);


static void Logger_LevelCheck_Disabled(benchmark::State& state)
{
	Logger& logger = Logger::get("BenchLogger.LevelCheck.Disabled");
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(logger.debug());  // Should be false
	}
}
BENCHMARK(Logger_LevelCheck_Disabled);


//
// Logger::get() - tests logger lookup performance
//

static void Logger_Get_Existing(benchmark::State& state)
{
	// Ensure logger exists first
	Logger::get("BenchLogger.GetExisting");

	for (auto _ : state)
	{
		Logger& logger = Logger::get("BenchLogger.GetExisting");
		benchmark::DoNotOptimize(&logger);
	}
}
BENCHMARK(Logger_Get_Existing);


static void Logger_Get_Hierarchical(benchmark::State& state)
{
	// Create a deep hierarchy
	Logger::get("BenchLogger.Hierarchy.Level1.Level2.Level3");

	for (auto _ : state)
	{
		Logger& logger = Logger::get("BenchLogger.Hierarchy.Level1.Level2.Level3");
		benchmark::DoNotOptimize(&logger);
	}
}
BENCHMARK(Logger_Get_Hierarchical);


//
// Message::init() component benchmarks - measuring individual syscall costs
//

static void Message_Init_ProcessId(benchmark::State& state)
{
	for (auto _ : state)
	{
		long pid = Process::id();
		benchmark::DoNotOptimize(pid);
	}
}
BENCHMARK(Message_Init_ProcessId);


static void Message_Init_CurrentOsTid(benchmark::State& state)
{
	for (auto _ : state)
	{
		long ostid = Thread::currentOsTid();
		benchmark::DoNotOptimize(ostid);
	}
}
BENCHMARK(Message_Init_CurrentOsTid);


static void Message_Init_ThreadCurrent(benchmark::State& state)
{
	for (auto _ : state)
	{
		Thread* pThread = Thread::current();
		benchmark::DoNotOptimize(pThread);
	}
}
BENCHMARK(Message_Init_ThreadCurrent);


static void Message_Init_Timestamp(benchmark::State& state)
{
	for (auto _ : state)
	{
		Timestamp ts;
		benchmark::DoNotOptimize(ts.epochMicroseconds());
	}
}
BENCHMARK(Message_Init_Timestamp);


//
// String copy benchmarks - measuring allocation overhead
//

static void String_Copy_Short(benchmark::State& state)
{
	std::string source = "TestSource";  // 10 chars - within SSO

	for (auto _ : state)
	{
		std::string copy = source;
		benchmark::DoNotOptimize(copy.data());
	}
}
BENCHMARK(String_Copy_Short);


static void String_Copy_Medium(benchmark::State& state)
{
	std::string text = "This is a test log message";  // 26 chars - likely heap

	for (auto _ : state)
	{
		std::string copy = text;
		benchmark::DoNotOptimize(copy.data());
	}
}
BENCHMARK(String_Copy_Medium);


static void String_Copy_Both(benchmark::State& state)
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
BENCHMARK(String_Copy_Both);


//
// Message creation benchmarks
//

static void Message_Create_Copy(benchmark::State& state)
{
	std::string source = "TestSource";
	std::string text = "This is a test log message";

	for (auto _ : state)
	{
		Message msg(source, text, Message::PRIO_INFORMATION);
		benchmark::DoNotOptimize(msg.getText().data());
	}
}
BENCHMARK(Message_Create_Copy);


static void Message_Create_Move(benchmark::State& state)
{
	std::string source = "TestSource";

	for (auto _ : state)
	{
		std::string text = "This is a test log message";
		Message msg(source, std::move(text), Message::PRIO_INFORMATION);
		benchmark::DoNotOptimize(msg.getText().data());
	}
}
BENCHMARK(Message_Create_Move);


static void Message_Create_WithSourceFile(benchmark::State& state)
{
	std::string source = "TestSource";
	std::string text = "This is a test log message";

	for (auto _ : state)
	{
		Message msg(source, text, Message::PRIO_INFORMATION, __FILE__, __LINE__);
		benchmark::DoNotOptimize(msg.getText().data());
	}
}
BENCHMARK(Message_Create_WithSourceFile);


static void Message_Copy(benchmark::State& state)
{
	Message original("TestSource", "This is a test log message", Message::PRIO_INFORMATION, __FILE__, __LINE__);

	for (auto _ : state)
	{
		Message copy(original);
		benchmark::DoNotOptimize(copy.getText().data());
	}
}
BENCHMARK(Message_Copy);


static void Message_Move(benchmark::State& state)
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
BENCHMARK(Message_Move);


//
// Logging with NullChannel - measures overhead without I/O
//

static void Logger_Log_NullChannel_Copy(benchmark::State& state)
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
BENCHMARK(Logger_Log_NullChannel_Copy);


static void Logger_Log_NullChannel_Move(benchmark::State& state)
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
BENCHMARK(Logger_Log_NullChannel_Move);


static void Logger_Log_NullChannel_CopyFresh(benchmark::State& state)
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
BENCHMARK(Logger_Log_NullChannel_CopyFresh);


static void Logger_Log_NullChannel_Literal(benchmark::State& state)
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
BENCHMARK(Logger_Log_NullChannel_Literal);


static void Logger_Log_NullChannel_WithSrcLocation(benchmark::State& state)
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
BENCHMARK(Logger_Log_NullChannel_WithSrcLocation);


static void Logger_Log_Disabled(benchmark::State& state)
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
BENCHMARK(Logger_Log_Disabled);


//
// Logging with FormattingChannel - tests the full pipeline
//

static void Logger_FormattingChannel_Simple(benchmark::State& state)
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
BENCHMARK(Logger_FormattingChannel_Simple);


static void Logger_FormattingChannel_Typical(benchmark::State& state)
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
BENCHMARK(Logger_FormattingChannel_Typical);


static void Logger_FormattingChannel_Full(benchmark::State& state)
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
BENCHMARK(Logger_FormattingChannel_Full);


static void Logger_FormattingChannel_Debug(benchmark::State& state)
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
BENCHMARK(Logger_FormattingChannel_Debug);


//
// Variadic template logging (format string)
//

static void Logger_Format_OneArg(benchmark::State& state)
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
BENCHMARK(Logger_Format_OneArg);


static void Logger_Format_ThreeArgs(benchmark::State& state)
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
BENCHMARK(Logger_Format_ThreeArgs);


static void Logger_Format_Disabled(benchmark::State& state)
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
BENCHMARK(Logger_Format_Disabled);


//
// Logger::format() static method (legacy $0, $1 format)
//

static void Logger_StaticFormat_OneArg(benchmark::State& state)
{
	std::string fmt = "User $0 logged in";
	std::string arg = "testuser";

	for (auto _ : state)
	{
		std::string result = Logger::format(fmt, arg);
		benchmark::DoNotOptimize(result.data());
	}
}
BENCHMARK(Logger_StaticFormat_OneArg);


static void Logger_StaticFormat_FourArgs(benchmark::State& state)
{
	std::string fmt = "User $0 performed $1 on $2 with result $3";

	for (auto _ : state)
	{
		std::string result = Logger::format(fmt, "testuser", "update", "item42", "success");
		benchmark::DoNotOptimize(result.data());
	}
}
BENCHMARK(Logger_StaticFormat_FourArgs);


//
// Hex dump formatting
//

static void Logger_FormatDump_Small(benchmark::State& state)
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
BENCHMARK(Logger_FormatDump_Small);


static void Logger_FormatDump_Medium(benchmark::State& state)
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
BENCHMARK(Logger_FormatDump_Medium);


static void Logger_FormatDump_Large(benchmark::State& state)
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
BENCHMARK(Logger_FormatDump_Large);


//
// Macro-based logging (poco_information, etc.)
// These macros include an if-check before constructing the message
//

static void Logger_Macro_Enabled(benchmark::State& state)
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
BENCHMARK(Logger_Macro_Enabled);


static void Logger_Macro_Disabled(benchmark::State& state)
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
BENCHMARK(Logger_Macro_Disabled);


static void Logger_Macro_Format_Enabled(benchmark::State& state)
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
BENCHMARK(Logger_Macro_Format_Enabled);


static void Logger_Macro_Format_Disabled(benchmark::State& state)
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
BENCHMARK(Logger_Macro_Format_Disabled);


//
// parseLevel - converting string level names to integers
//

static void Logger_ParseLevel(benchmark::State& state)
{
	for (auto _ : state)
	{
		int level = Logger::parseLevel("information");
		benchmark::DoNotOptimize(level);
	}
}
BENCHMARK(Logger_ParseLevel);


//
// Complete pipeline comparison benchmarks
//

static void Logger_Pipeline_DirectLog(benchmark::State& state)
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
BENCHMARK(Logger_Pipeline_DirectLog);


static void Logger_Pipeline_WithTypicalFormat(benchmark::State& state)
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
BENCHMARK(Logger_Pipeline_WithTypicalFormat);


static void Logger_Pipeline_WithLocalTime(benchmark::State& state)
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
BENCHMARK(Logger_Pipeline_WithLocalTime);


//
// AsyncChannel benchmarks - async logging with Poco's AsyncChannel
//
// Why AsyncChannel's wall time ≈ CPU time:
//
// AsyncChannel::log() does significant work in the calling thread:
//   1. Allocates a MessageNotification on the heap (new MessageNotification)
//   2. Copies the entire Message into the notification
//   3. Acquires a mutex to push onto the NotificationQueue
//
// All this happens synchronously before log() returns, so the producer
// thread's CPU time reflects most of the actual work. The backend thread
// just dequeues and forwards - it doesn't add much to wall time because
// it runs in parallel with the next log() call.
//

static void Logger_AsyncChannel_NullChannel(benchmark::State& state)
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
BENCHMARK(Logger_AsyncChannel_NullChannel);


static void Logger_AsyncChannel_WithFormat(benchmark::State& state)
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
BENCHMARK(Logger_AsyncChannel_WithFormat);


static void Logger_AsyncChannel_ToFile(benchmark::State& state)
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
BENCHMARK(Logger_AsyncChannel_ToFile);


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
// Why FastLogger's CPU time << wall time (contrast with AsyncChannel):
//
// FastLogger::log() does minimal work in the calling thread:
//   - Writes a few bytes to a pre-allocated lock-free ring buffer
//   - Returns immediately without any allocation or mutex
//
// The heavy lifting (string copying, formatting, I/O) all happens in the
// backend thread. This is why CPU time is very low but wall time includes
// the time waiting for the backend to process the message.
//

// Configure FastLogger backend options before any FastLogger is created.
// This must happen before FastLogger::get() is called for the first time.
struct FastLoggerBackendInitializer
{
	FastLoggerBackendInitializer()
	{
		// Disable yielding when idle for more consistent benchmark timing.
		// When enabled, the backend thread yields when no messages are pending,
		// which can add scheduling latency in benchmarks.
		Poco::FastLogger::setBackendOption("enableYieldWhenIdle", "false");
	}
};
static FastLoggerBackendInitializer fastLoggerBackendInitializer;


// Measures the cost of checking if a log level is enabled (returns true)
static void FastLogger_LevelCheck_Enabled(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.LevelCheck.Enabled");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(logger.information());
	}
}
BENCHMARK(FastLogger_LevelCheck_Enabled);


// Measures the cost of checking if a log level is enabled (returns false)
static void FastLogger_LevelCheck_Disabled(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.LevelCheck.Disabled");
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(logger.debug());  // Should be false
	}
}
BENCHMARK(FastLogger_LevelCheck_Disabled);


// Measures the cost of looking up an existing logger by name
static void FastLogger_Get_Existing(benchmark::State& state)
{
	// Ensure logger exists first
	Poco::FastLogger::get("BenchFastLogger.GetExisting");

	for (auto _ : state)
	{
		Poco::FastLogger& l = Poco::FastLogger::get("BenchFastLogger.GetExisting");
		benchmark::DoNotOptimize(&l);
	}
}
BENCHMARK(FastLogger_Get_Existing);


// Measures logging overhead with null sink (no I/O)
// This isolates the pure queueing cost
static void FastLogger_Log_NullChannel(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.NullChannel");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");
	}
}
BENCHMARK(FastLogger_Log_NullChannel);


// Measures the cost when logging is disabled by level
// Should be near-zero as the call short-circuits before queueing
static void FastLogger_Log_Disabled(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.Disabled");
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		// This should short-circuit immediately due to level check
		logger.information("This is a test log message");
	}
}
BENCHMARK(FastLogger_Log_Disabled);


// Measures logging a string literal (no dynamic allocation)
static void FastLogger_Log_Literal(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.Literal");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("This is a test log message");  // String literal
	}
}
BENCHMARK(FastLogger_Log_Literal);


// Measures logging with one format argument
// Includes Poco::format() overhead in the calling thread
static void FastLogger_Format_OneArg(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.Format.OneArg");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("User %s logged in", std::string("testuser"));
	}
}
BENCHMARK(FastLogger_Format_OneArg);


// Measures logging with three format arguments
// More format args = more work in calling thread
static void FastLogger_Format_ThreeArgs(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.Format.ThreeArgs");
	logger.setLevel(Message::PRIO_TRACE);

	for (auto _ : state)
	{
		logger.information("User %s performed %s on item %d", std::string("testuser"), std::string("update"), 42);
	}
}
BENCHMARK(FastLogger_Format_ThreeArgs);


// Verifies that format arguments are NOT evaluated when level is disabled
// Should be near-zero if level check happens before format evaluation
static void FastLogger_Format_Disabled(benchmark::State& state)
{
	Poco::FastLogger& logger = Poco::FastLogger::get("BenchFastLogger.Format.Disabled");
	logger.setLevel(Message::PRIO_ERROR);  // Only ERROR and above

	for (auto _ : state)
	{
		// Format arguments should NOT be evaluated due to level check
		logger.information("User %s performed %s on item %d", std::string("testuser"), std::string("update"), 42);
	}
}
BENCHMARK(FastLogger_Format_Disabled);


// Measures logging to an actual file
// Note: Setup overhead is included; use fixture version for accurate comparison
static void FastLogger_ToFile(benchmark::State& state)
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
BENCHMARK(FastLogger_ToFile);


//
// Direct comparison: Logger+AsyncChannel vs FastLogger
//
// These benchmarks compare the two async logging approaches writing to files.
// Both are configured via LoggingConfigurator for comparable setups.
//
// Expected results:
//   - CPU time: FastLogger should be significantly faster (less work in hot path)
//

// Common configuration templates for fair comparison
static const char* asyncChannelFileConfig =
	"logging.loggers.l1.name = BenchComparison.Async.File\n"
	"logging.loggers.l1.level = trace\n"
	"logging.loggers.l1.channel = c1\n"
	"logging.channels.c1.class = AsyncChannel\n"
	"logging.channels.c1.channel = c2\n"
	"logging.channels.c2.class = FileChannel\n"
	"logging.channels.c2.path = %s\n"
	"logging.channels.c2.formatter = f1\n"
	"logging.formatters.f1.class = PatternFormatter\n"
	"logging.formatters.f1.pattern = %s\n";

static const char* fastLoggerFileConfig =
	"logging.loggers.l1.name = BenchComparison.Fast.File\n"
	"logging.loggers.l1.type = fast\n"
	"logging.loggers.l1.level = trace\n"
	"logging.loggers.l1.channel = c1\n"
	"logging.channels.c1.class = FileChannel\n"
	"logging.channels.c1.path = %s\n"
	"logging.channels.c1.formatter = f1\n"
	"logging.formatters.f1.class = PatternFormatter\n"
	"logging.formatters.f1.pattern = %s\n";

static const std::string benchPattern = "%Y-%m-%d %H:%M:%S.%i [%p] %s: %t";

// Logger + AsyncChannel writing to file
// Uses mutex-based queue internally
static void Loggers_AsyncChannel_File(benchmark::State& state)
{
	std::string tempFile = TemporaryFile::tempName() + "_async.log";
	LoggingConfigurator::configure("trace", benchPattern,
		Poco::format(std::string(asyncChannelFileConfig), tempFile, benchPattern));

	Logger& logger = Logger::get("BenchComparison.Async.File");

	for (auto _ : state)
	{
		logger.information("This is a test log message for comparison");
	}

	logger.getChannel()->close();
	try { File(tempFile).remove(); } catch (...) {}
}
BENCHMARK(Loggers_AsyncChannel_File);


// FastLogger writing to file
// Uses lock-free SPSC queue internally (Quill)
static void Loggers_FastLogger_File(benchmark::State& state)
{
	std::string tempFile = TemporaryFile::tempName() + "_fast.log";
	LoggingConfigurator::configure("trace", benchPattern,
		Poco::format(std::string(fastLoggerFileConfig), tempFile, benchPattern));

	Poco::FastLogger& logger = Poco::FastLogger::get("BenchComparison.Fast.File");

	for (auto _ : state)
	{
		logger.information("This is a test log message for comparison");
	}

	try { File(tempFile).remove(); } catch (...) {}
}
BENCHMARK(Loggers_FastLogger_File);


//
// Throughput comparison with varying message sizes (null sink)
//
// These tests isolate queue/threading overhead by discarding output.
// Short messages test minimum overhead; long messages test memory/copy costs.
//

static const char* asyncChannelNullConfig =
	"logging.loggers.l1.name = %s\n"
	"logging.loggers.l1.level = trace\n"
	"logging.loggers.l1.channel = c1\n"
	"logging.channels.c1.class = AsyncChannel\n"
	"logging.channels.c1.channel = c2\n"
	"logging.channels.c2.class = NullChannel\n";

static const char* fastLoggerNullConfig =
	"logging.loggers.l1.name = %s\n"
	"logging.loggers.l1.type = fast\n"
	"logging.loggers.l1.level = trace\n";
	// FastLogger uses null sink by default when no channel is configured

// AsyncChannel with short message (~9 bytes)
static void Loggers_AsyncChannel_ShortMsg(benchmark::State& state)
{
	LoggingConfigurator::configure("trace", "%t",
		Poco::format(std::string(asyncChannelNullConfig), std::string("BenchComparison.Async.Short")));

	Logger& logger = Logger::get("BenchComparison.Async.Short");

	for (auto _ : state)
	{
		logger.information("Short msg");
	}

	logger.getChannel()->close();
}
BENCHMARK(Loggers_AsyncChannel_ShortMsg);


// FastLogger with short message (~9 bytes)
static void Loggers_FastLogger_ShortMsg(benchmark::State& state)
{
	LoggingConfigurator::configure("trace", "%t",
		Poco::format(std::string(fastLoggerNullConfig), std::string("BenchComparison.Fast.Short")));

	Poco::FastLogger& logger = Poco::FastLogger::get("BenchComparison.Fast.Short");

	for (auto _ : state)
	{
		logger.information("Short msg");
	}
}
BENCHMARK(Loggers_FastLogger_ShortMsg);


// AsyncChannel with long message (256 bytes)
// Tests how message size affects queueing overhead
static void Loggers_AsyncChannel_LongMsg(benchmark::State& state)
{
	LoggingConfigurator::configure("trace", "%t",
		Poco::format(std::string(asyncChannelNullConfig), std::string("BenchComparison.Async.Long")));

	Logger& logger = Logger::get("BenchComparison.Async.Long");

	std::string longMsg(256, 'x');
	for (auto _ : state)
	{
		logger.information(longMsg);
	}

	logger.getChannel()->close();
}
BENCHMARK(Loggers_AsyncChannel_LongMsg);


// FastLogger with long message (256 bytes)
// Tests how message size affects queueing overhead
static void Loggers_FastLogger_LongMsg(benchmark::State& state)
{
	LoggingConfigurator::configure("trace", "%t",
		Poco::format(std::string(fastLoggerNullConfig), std::string("BenchComparison.Fast.Long")));

	Poco::FastLogger& logger = Poco::FastLogger::get("BenchComparison.Fast.Long");

	std::string longMsg(256, 'x');
	for (auto _ : state)
	{
		logger.information(longMsg);
	}
}
BENCHMARK(Loggers_FastLogger_LongMsg);


#endif // POCO_ENABLE_FASTLOGGER


} // namespace
