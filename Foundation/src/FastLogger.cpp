//
// FastLogger.cpp
//
// Library: Foundation
// Package: Logging
// Module:  FastLogger
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Foundation.h"


#ifdef POCO_ENABLE_FASTLOGGER


#include "Poco/FastLogger.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/LoggingRegistry.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/NullChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/AsyncChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/EventChannel.h"
#include "Poco/StreamChannel.h"
#include "Poco/Environment.h"
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOG_CHANNEL)
#include "Poco/SyslogChannel.h"
#endif
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/EventLogChannel.h"
#include "Poco/WindowsConsoleChannel.h"
#endif
#include <memory>
#include <iostream>

// On Unix with syslog support, we need to include SyslogSink.h which pulls in <syslog.h>.
// syslog.h defines macros like LOG_INFO, LOG_WARNING, etc. that conflict with Quill's
// LOG_* macros. We must define QUILL_DISABLE_NON_PREFIXED_MACROS before including any
// Quill headers, and then use QUILL_LOG_* macros instead.
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOG_CHANNEL)
#define QUILL_DISABLE_NON_PREFIXED_MACROS
#endif

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include "quill/sinks/FileSink.h"
#include "quill/sinks/RotatingFileSink.h"
#include "quill/sinks/Sink.h"
#include "quill/sinks/StreamSink.h"
#include "quill/core/FrontendOptions.h"
#include "quill/core/PatternFormatterOptions.h"
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOG_CHANNEL)
#include "quill/sinks/SyslogSink.h"
#endif

#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <atomic>

#ifdef __linux__
#include <unistd.h>  // for sysconf
#endif


namespace Poco {


// Custom frontend options for Poco::FastLogger
// Uses an unbounded blocking queue with a reasonable max capacity to prevent
// extreme memory growth while ensuring no messages are dropped
struct PocoFrontendOptions
{
	// Use UnboundedBlocking - blocks when queue is full (no message loss)
	// This is fairer for benchmarking as it matches AsyncChannel's blocking behavior
	static constexpr quill::QueueType queue_type = quill::QueueType::UnboundedBlocking;

	// Start with 256KB queue (default is 128KB)
	static constexpr size_t initial_queue_capacity = 256u * 1024u;

	// Retry interval when queue is full (in nanoseconds) - short for low latency
	static constexpr uint32_t blocking_queue_retry_interval_ns = 100;

	// Max capacity of 64MB - prevents runaway memory growth (default is 2GB)
	static constexpr size_t unbounded_queue_max_capacity = 64u * 1024u * 1024u;

	// Huge pages policy
	static constexpr quill::HugePagesPolicy huge_pages_policy = quill::HugePagesPolicy::Never;
};

// Type alias for the Poco-specific logger
using PocoQuillLogger = quill::LoggerImpl<PocoFrontendOptions>;
using PocoQuillFrontend = quill::FrontendImpl<PocoFrontendOptions>;


const std::string FastLogger::ROOT;

FastLogger::LoggerMapPtr FastLogger::_pLoggerMap;
Mutex FastLogger::_mapMtx;
bool FastLogger::_backendStarted = false;
Mutex FastLogger::_backendMtx;

// Static storage for backend options set before backend starts
namespace {
	std::map<std::string, std::string> _pendingBackendOptions;
	Mutex _backendOptionsMtx;
}


namespace {



// NullSink - discards all log output (for benchmarking/testing)
class NullSink : public quill::Sink
{
public:
	NullSink() = default;

	void write_log(quill::MacroMetadata const* /* log_metadata */,
	               uint64_t /* log_timestamp */,
	               std::string_view /* thread_id */,
	               std::string_view /* thread_name */,
	               std::string const& /* process_id */,
	               std::string_view /* logger_name */,
	               quill::LogLevel /* log_level */,
	               std::string_view /* log_level_description */,
	               std::string_view /* log_level_short_code */,
	               std::vector<std::pair<std::string, std::string>> const* /* named_args */,
	               std::string_view /* log_message */,
	               std::string_view /* log_statement */) override
	{
		// Discard all output
	}

	void flush_sink() override
	{
		// Nothing to flush
	}
};


// Global console sink shared by all loggers
std::shared_ptr<quill::Sink> getConsoleSink()
{
	static auto sink = PocoQuillFrontend::create_or_get_sink<quill::ConsoleSink>("poco_console");
	return sink;
}


// Global null sink shared by all loggers that use NullChannel
std::shared_ptr<quill::Sink> getNullSink()
{
	static auto sink = PocoQuillFrontend::create_or_get_sink<NullSink>("poco_null");
	return sink;
}


std::shared_ptr<quill::Sink> getDefaultSink()
{
	// Default to null sink (silent), matching Logger's behavior.
	// Messages are discarded until setChannel() is called.
	return getNullSink();
}


// Cached hostname for %N pattern specifier
std::string _cachedHostname;

const std::string& getCachedHostname()
{
	if (_cachedHostname.empty())
	{
		_cachedHostname = Environment::nodeName();
	}
	return _cachedHostname;
}


// Translate Poco PatternFormatter pattern to a Quill format/timestamp pattern.
struct PatternTranslation
{
	std::string formatPattern;     // Quill format pattern
	std::string timestampPattern;  // Quill timestamp pattern
	bool localTime = false;        // true if the Poco pattern carried a %L flag
};

PatternTranslation translatePocoPatternToQuill(const std::string& pocoPattern)
{
	// Quill format pattern attributes:
	// %(time), %(file_name), %(full_path), %(caller_function), %(log_level),
	// %(log_level_short_code), %(line_number), %(logger), %(message),
	// %(thread_id), %(thread_name), %(process_id), %(source_location), %(short_source_location)
	//
	// Poco pattern specifiers:
	// %s - source, %t - text, %l - level number, %p - priority name, %q - priority abbrev
	// %P - process id, %T - thread name, %I - thread id, %N - hostname
	// %U - source file path, %O - source file basename, %u - source line
	// Date/time: %Y, %m, %d, %H, %M, %S, %i (ms), %F (us), etc.
	//
	// Quill renders the whole timestamp through a single %(time) attribute whose
	// layout is a separate timestamp_pattern. Poco patterns interleave date/time
	// specifiers with their separators and with non-time fields, e.g.
	//   %Y-%m-%d %H:%M:%S.%i [%p] %s: %t
	// A contiguous run of date/time specifiers (including the separators between
	// them) goes into the timestamp pattern, with one %(time) emitted in the
	// format pattern at the run's position; everything else goes to the format
	// pattern. Literals inside a run are buffered until the next token decides
	// whether they are intra-timestamp separators or a trailing literal.

	std::string quillPattern;
	std::string timestampPattern;
	std::string pendingLiterals;   // literals inside a run, routed once the run continues or ends
	bool inTimestamp = false;      // currently inside a date/time run
	bool emittedTime = false;      // a %(time) has been emitted
	bool localTimeFlag = false;

	quillPattern.reserve(pocoPattern.size() * 2);
	timestampPattern.reserve(32);

	// Begin or continue a date/time run, appending token to the timestamp pattern.
	auto onDateField = [&](const char* token)
	{
		if (!inTimestamp)
		{
			quillPattern += "%(time)";
			inTimestamp = true;
			emittedTime = true;
		}
		else
		{
			// literals since the previous date field were intra-timestamp separators
			timestampPattern += pendingLiterals;
			pendingLiterals.clear();
		}
		timestampPattern += token;
	};

	// End an active run: literals buffered after the last date field belong to
	// the surrounding line layout (they follow %(time) in the format pattern).
	auto endTimestampRun = [&]()
	{
		if (inTimestamp)
		{
			quillPattern += pendingLiterals;
			pendingLiterals.clear();
			inTimestamp = false;
		}
	};

	std::string::const_iterator it = pocoPattern.begin();
	std::string::const_iterator end = pocoPattern.end();

	while (it != end)
	{
		if (*it != '%')
		{
			if (inTimestamp) pendingLiterals += *it;
			else quillPattern += *it;
			++it;
			continue;
		}

		++it;
		if (it == end)
		{
			quillPattern += '%';
			break;
		}

		char spec = *it;
		++it;

		switch (spec)
		{
		// Date/time specifiers - part of the timestamp run.
		case 'Y':  // year with century
		case 'y':  // year without century
		case 'm':  // month (01-12)
		case 'n':  // month (1-12)
		case 'd':  // day (01-31)
		case 'e':  // day (1-31)
		case 'H':  // hour (00-23)
		case 'h':  // hour (01-12)
		case 'M':  // minute
		case 'S':  // second
		case 'a':  // am/pm
		case 'A':  // AM/PM
		case 'w':  // weekday abbrev
		case 'W':  // weekday full
		case 'b':  // month abbrev
		case 'B':  // month full
		case 'z':  // timezone ISO
		case 'Z':  // timezone RFC
			{
				char tok = spec;
				if (spec == 'n') tok = 'm';        // Quill uses standard strftime
				else if (spec == 'e') tok = 'd';
				const char token[3] = { '%', tok, '\0' };
				onDateField(token);
			}
			break;

		case 'i':  // milliseconds
			onDateField("%Qms");
			break;
		case 'c':  // centiseconds - approximate with milliseconds
			onDateField("%Qms");
			break;
		case 'F':  // microseconds
			onDateField("%Qus");
			break;

		case 'E':  // epoch time - not directly supported; emit %(time), no token
			if (!inTimestamp)
			{
				quillPattern += "%(time)";
				inTimestamp = true;
				emittedTime = true;
			}
			break;

		case 'L':  // Poco local-time flag - emits no token and does not bound
			// the run; mapped to Quill's timestamp timezone.
			localTimeFlag = true;
			break;

		// Message content
		case 't':  // message text
			endTimestampRun(); quillPattern += "%(message)";
			break;
		case 's':  // message source (logger name)
			endTimestampRun(); quillPattern += "%(logger)";
			break;

		// Log level
		case 'l':  // level number - Quill doesn't have this, use short code
			endTimestampRun(); quillPattern += "%(log_level_short_code)";
			break;
		case 'p':  // priority name (Fatal, Critical, etc.)
			endTimestampRun(); quillPattern += "%(log_level)";
			break;
		case 'q':  // abbreviated priority (F, C, E, W, N, I, D, T)
			endTimestampRun(); quillPattern += "%(log_level_short_code)";
			break;

		// Process/Thread info
		case 'P':  // process id
			endTimestampRun(); quillPattern += "%(process_id)";
			break;
		case 'T':  // thread name
			endTimestampRun(); quillPattern += "%(thread_name)";
			break;
		case 'I':  // thread id
			endTimestampRun(); quillPattern += "%(thread_id)";
			break;
		case 'J':  // OS thread id - use thread_id as fallback
			endTimestampRun(); quillPattern += "%(thread_id)";
			break;
		case 'N':  // hostname - hardcoded from cached value
			endTimestampRun(); quillPattern += getCachedHostname();
			break;

		// Source location
		case 'U':  // source file path
			endTimestampRun(); quillPattern += "%(full_path)";
			break;
		case 'O':  // source file basename
			endTimestampRun(); quillPattern += "%(file_name)";
			break;
		case 'u':  // source line number
			endTimestampRun(); quillPattern += "%(line_number)";
			break;

		case 'v':  // source with width - just use logger name
			endTimestampRun();
			// Skip optional width specifier [n]
			if (it != end && *it == '[')
			{
				while (it != end && *it != ']') ++it;
				if (it != end) ++it;
			}
			quillPattern += "%(logger)";
			break;

		case '[':  // custom property %[name] - not supported in Quill
			endTimestampRun();
			// Skip until ]
			while (it != end && *it != ']') ++it;
			if (it != end) ++it;
			quillPattern += "[prop]";
			break;

		case '%':  // literal %
			if (inTimestamp) pendingLiterals += '%';
			else quillPattern += '%';
			break;

		default:
			// Unknown specifier, pass through to the format pattern.
			endTimestampRun();
			quillPattern += '%';
			quillPattern += spec;
			break;
		}
	}

	// Flush any trailing literals that followed the last date field.
	endTimestampRun();

	// If no timestamp pattern was built, use a sensible default
	if (timestampPattern.empty() && emittedTime)
	{
		timestampPattern = "%Y-%m-%d %H:%M:%S.%Qms";
	}
	else if (timestampPattern.empty())
	{
		timestampPattern = "%H:%M:%S.%Qns";  // Quill default
	}

	PatternTranslation result;
	result.formatPattern = quillPattern;
	result.timestampPattern = timestampPattern;
	result.localTime = localTimeFlag;
	return result;
}


// Structure to hold collected sink info including formatter
struct SinkInfo
{
	std::vector<std::shared_ptr<quill::Sink>> sinks;
	std::string formatPattern;      // Quill format pattern (empty = default)
	std::string timestampPattern;   // Quill timestamp pattern (empty = default)
	// Defaults to local time; a configured PatternFormatter "times" or a %L flag overrides it.
	quill::Timezone timestampTimezone = quill::Timezone::LocalTime;
};


// Forward declaration for recursive sink collection
void collectSinksFromChannel(Channel::Ptr pChannel, SinkInfo& info, std::unordered_set<const Channel*>& visited);


// Structure to hold rotation configuration extracted from Poco FileChannel
struct RotationConfig
{
	bool enabled = false;
	size_t maxFileSize = 0;                  // 0 = disabled
	bool dailyRotation = false;
	std::string dailyRotationTime;           // "HH:MM" format
	char frequencyType = 0;                  // 'M' for minutes, 'H' for hours, 0 = disabled
	uint32_t frequencyInterval = 0;
	uint32_t maxBackupFiles = 0;             // 0 = unlimited
	bool rotateOnOpen = false;
	quill::RotatingFileSinkConfig::RotationNamingScheme namingScheme =
		quill::RotatingFileSinkConfig::RotationNamingScheme::Index;
};


// Parse Poco rotation string and populate RotationConfig
// Poco rotation formats:
//   - "never" or empty: no rotation
//   - "<n>": size in bytes
//   - "<n> K": size in kilobytes
//   - "<n> M": size in megabytes
//   - "<n> minutes": rotate every n minutes
//   - "<n> hours": rotate every n hours
//   - "<n> days": rotate every n days
//   - "daily": rotate daily
//   - "weekly": rotate weekly
//   - "monthly": rotate monthly
//   - "HH:MM" or "day,HH:MM": rotate at specific time
RotationConfig parsePocoRotation(const std::string& rotation, const std::string& archive,
                                  const std::string& purgeCount, bool rotateOnOpen)
{
	RotationConfig config;
	config.rotateOnOpen = rotateOnOpen;

	if (rotation.empty() || icompare(rotation, "never") == 0)
	{
		return config;
	}

	// Parse archive strategy for naming scheme
	if (icompare(archive, "timestamp") == 0)
	{
		config.namingScheme = quill::RotatingFileSinkConfig::RotationNamingScheme::DateAndTime;
	}
	else
	{
		// "number" or default
		config.namingScheme = quill::RotatingFileSinkConfig::RotationNamingScheme::Index;
	}

	// Parse purgeCount for max backup files
	if (!purgeCount.empty() && icompare(purgeCount, "none") != 0)
	{
		int count = 0;
		if (NumberParser::tryParse(purgeCount, count) && count > 0)
		{
			config.maxBackupFiles = static_cast<uint32_t>(count);
		}
	}

	// Check for time-based rotation patterns
	// Daily rotation at specific time: "HH:MM" or "day,HH:MM"
	if (rotation.find(':') != std::string::npos)
	{
		// Extract time part (last HH:MM)
		std::string timePart = rotation;
		size_t commaPos = rotation.find(',');
		if (commaPos != std::string::npos)
		{
			timePart = rotation.substr(commaPos + 1);
		}
		// Trim whitespace
		while (!timePart.empty() && std::isspace(static_cast<unsigned char>(timePart.front())))
			timePart.erase(0, 1);
		while (!timePart.empty() && std::isspace(static_cast<unsigned char>(timePart.back())))
			timePart.pop_back();

		config.enabled = true;
		config.dailyRotation = true;
		config.dailyRotationTime = timePart;
		return config;
	}

	// Check for keyword-based rotation
	if (icompare(rotation, "daily") == 0)
	{
		config.enabled = true;
		config.dailyRotation = true;
		config.dailyRotationTime = "00:00";
		return config;
	}
	if (icompare(rotation, "weekly") == 0)
	{
		// Quill doesn't support weekly directly, use daily as approximation
		config.enabled = true;
		config.dailyRotation = true;
		config.dailyRotationTime = "00:00";
		return config;
	}
	if (icompare(rotation, "monthly") == 0)
	{
		// Quill doesn't support monthly directly, use daily as approximation
		config.enabled = true;
		config.dailyRotation = true;
		config.dailyRotationTime = "00:00";
		return config;
	}

	// Parse numeric value with optional unit
	std::string::const_iterator it = rotation.begin();
	std::string::const_iterator end = rotation.end();

	// Skip leading whitespace
	while (it != end && std::isspace(static_cast<unsigned char>(*it))) ++it;

	// Parse number
	std::string numStr;
	while (it != end && std::isdigit(static_cast<unsigned char>(*it)))
	{
		numStr += *it++;
	}

	if (numStr.empty())
	{
		return config;  // Invalid format
	}

	int64_t value = 0;
	if (!NumberParser::tryParse64(numStr, value) || value <= 0)
	{
		return config;  // Invalid number
	}

	// Skip whitespace between number and unit
	while (it != end && std::isspace(static_cast<unsigned char>(*it))) ++it;

	// Parse unit
	std::string unit;
	while (it != end && std::isalpha(static_cast<unsigned char>(*it)))
	{
		unit += static_cast<char>(std::tolower(static_cast<unsigned char>(*it)));
		++it;
	}

	config.enabled = true;

	if (unit.empty())
	{
		// Pure number = size in bytes
		config.maxFileSize = static_cast<size_t>(value);
	}
	else if (unit == "k")
	{
		// Kilobytes
		config.maxFileSize = static_cast<size_t>(value) * 1024;
	}
	else if (unit == "m")
	{
		// Megabytes
		config.maxFileSize = static_cast<size_t>(value) * 1024 * 1024;
	}
	else if (unit == "minutes" || unit == "minute")
	{
		config.frequencyType = 'M';
		config.frequencyInterval = static_cast<uint32_t>(value);
	}
	else if (unit == "hours" || unit == "hour")
	{
		config.frequencyType = 'H';
		config.frequencyInterval = static_cast<uint32_t>(value);
	}
	else if (unit == "days" || unit == "day")
	{
		// Convert days to hours
		config.frequencyType = 'H';
		config.frequencyInterval = static_cast<uint32_t>(value * 24);
	}
	else if (unit == "weeks" || unit == "week")
	{
		// Convert weeks to hours
		config.frequencyType = 'H';
		config.frequencyInterval = static_cast<uint32_t>(value * 24 * 7);
	}
	else if (unit == "months" || unit == "month")
	{
		// Convert months (30 days) to hours
		config.frequencyType = 'H';
		config.frequencyInterval = static_cast<uint32_t>(value * 24 * 30);
	}
	else
	{
		// Unknown unit, treat as bytes
		config.maxFileSize = static_cast<size_t>(value);
	}

	return config;
}


// Helper to create a simple (non-rotating) file sink
std::shared_ptr<quill::Sink> createFileSink(const std::string& filePath)
{
	return PocoQuillFrontend::create_or_get_sink<quill::FileSink>(
		filePath,
		[]() {
			quill::FileSinkConfig cfg;
			cfg.set_open_mode('a');  // Append mode
			return cfg;
		}()
	);
}


// Helper to create a rotating file sink with configuration
std::shared_ptr<quill::Sink> createRotatingFileSink(const std::string& filePath, const RotationConfig& rotConfig)
{
	quill::RotatingFileSinkConfig cfg;
	cfg.set_open_mode('a');  // Append mode
	cfg.set_rotation_naming_scheme(rotConfig.namingScheme);

	if (rotConfig.rotateOnOpen)
	{
		cfg.set_rotation_on_creation(true);
	}

	if (rotConfig.maxBackupFiles > 0)
	{
		cfg.set_max_backup_files(rotConfig.maxBackupFiles);
	}

	if (rotConfig.maxFileSize > 0)
	{
		// Quill requires minimum 512 bytes
		size_t fileSize = std::max(rotConfig.maxFileSize, static_cast<size_t>(512));
		cfg.set_rotation_max_file_size(fileSize);
	}

	if (rotConfig.dailyRotation && !rotConfig.dailyRotationTime.empty())
	{
		cfg.set_rotation_time_daily(rotConfig.dailyRotationTime);
	}
	else if (rotConfig.frequencyType != 0 && rotConfig.frequencyInterval > 0)
	{
		cfg.set_rotation_frequency_and_interval(rotConfig.frequencyType, rotConfig.frequencyInterval);
	}

	return PocoQuillFrontend::create_or_get_sink<quill::RotatingFileSink>(
		filePath,
		cfg,
		quill::FileEventNotifier{}
	);
}


#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOG_CHANNEL)
// Map Poco syslog facility to system syslog facility
int pocoToSyslogFacility(int pocoFacility)
{
	// Poco uses the same values as syslog.h
	return pocoFacility;
}
#endif


// Recursively collect Quill sinks from a Poco channel hierarchy
void collectSinksFromChannel(Channel::Ptr pChannel, SinkInfo& info, std::unordered_set<const Channel*>& visited)
{
	if (!pChannel) return;

	// Guard against cycles in the channel graph: a SplitterChannel can
	// transitively contain itself (e.g. splitter.channels = splitter), which
	// would otherwise recurse until the stack overflows.
	if (!visited.insert(pChannel.get()).second) return;

	// Unwrap FormattingChannel - extract formatter pattern
	if (FormattingChannel* pFormattingChannel = dynamic_cast<FormattingChannel*>(pChannel.get()))
	{
		// Try to extract pattern from PatternFormatter
		if (info.formatPattern.empty())
		{
			Formatter::Ptr pFormatter = pFormattingChannel->getFormatter();
			if (pFormatter)
			{
				PatternFormatter* pPatternFormatter = dynamic_cast<PatternFormatter*>(pFormatter.get());
				if (pPatternFormatter)
				{
					std::string pocoPattern = pPatternFormatter->getProperty(PatternFormatter::PROP_PATTERN);
					if (!pocoPattern.empty())
					{
						auto translated = translatePocoPatternToQuill(pocoPattern);
						info.formatPattern = translated.formatPattern;
						info.timestampPattern = translated.timestampPattern;

						// FastLogger defaults to local time, unlike a regular
						// Logger/PatternFormatter, which defaults to UTC. Precedence
						// matches Poco's PatternFormatter: a %L flag forces local;
						// otherwise a configured "times" (local/UTC) is honored;
						// otherwise the local default stands. The formatter's reported
						// "UTC" is also its unset default, so isLocalTimeConfigured()
						// is needed to tell a configured UTC from an unset one.
						if (translated.localTime)
							info.timestampTimezone = quill::Timezone::LocalTime;
						else if (pPatternFormatter->isLocalTimeConfigured())
							info.timestampTimezone = pPatternFormatter->getLocalTime()
								? quill::Timezone::LocalTime
								: quill::Timezone::GmtTime;
					}
				}
			}
		}
		collectSinksFromChannel(pFormattingChannel->getChannel(), info, visited);
		return;
	}

	// AsyncChannel - FastLogger is already async, just forward to inner channel
	if (AsyncChannel* pAsyncChannel = dynamic_cast<AsyncChannel*>(pChannel.get()))
	{
		collectSinksFromChannel(pAsyncChannel->getChannel(), info, visited);
		return;
	}

	// SplitterChannel - recurse into each attached channel so a single
	// root.channel = splitter (e.g. console + file) maps to multiple Quill
	// sinks, and per-child PatternFormatter patterns are picked up.
	//
	// Limitation: all sinks collected from a splitter share one format/timezone
	// (the first child that supplies a PatternFormatter -- see the info.formatPattern
	// guard above), so per-child distinct formatting is not reproduced under
	// type=fast; use separate loggers if children need different formats.
	if (SplitterChannel* pSplitter = dynamic_cast<SplitterChannel*>(pChannel.get()))
	{
		const int n = pSplitter->count();
		for (int i = 0; i < n; ++i)
			collectSinksFromChannel(pSplitter->getChannel(i), info, visited);
		return;
	}

	// NullChannel
	if (dynamic_cast<NullChannel*>(pChannel.get()))
	{
		info.sinks.push_back(getNullSink());
		return;
	}

	// ConsoleChannel or ColorConsoleChannel
	if (dynamic_cast<ConsoleChannel*>(pChannel.get()) ||
	    dynamic_cast<ColorConsoleChannel*>(pChannel.get()))
	{
		info.sinks.push_back(getConsoleSink());
		return;
	}

	// FileChannel - with rotation support
	if (FileChannel* pFileChannel = dynamic_cast<FileChannel*>(pChannel.get()))
	{
		std::string filePath = pFileChannel->path();
		if (!filePath.empty())
		{
			// Extract rotation properties from the FileChannel
			std::string rotation = pFileChannel->getProperty(FileChannel::PROP_ROTATION);
			std::string archive = pFileChannel->getProperty(FileChannel::PROP_ARCHIVE);
			std::string purgeCount = pFileChannel->getProperty(FileChannel::PROP_PURGECOUNT);
			std::string rotateOnOpenStr = pFileChannel->getProperty(FileChannel::PROP_ROTATEONOPEN);
			bool rotateOnOpen = (icompare(rotateOnOpenStr, "true") == 0);

			// Check for compress property - not supported by Quill
			std::string compress = pFileChannel->getProperty(FileChannel::PROP_COMPRESS);
			if (icompare(compress, "true") == 0)
			{
				std::cerr << "FastLogger warning: compress property is not supported. "
				          << "Rotated log files will not be compressed. "
				          << "Use external compression tools if needed." << std::endl;
			}

			// Check for purgeAge property - not supported by Quill (only purgeCount is supported)
			std::string purgeAge = pFileChannel->getProperty(FileChannel::PROP_PURGEAGE);
			if (!purgeAge.empty())
			{
				std::cerr << "FastLogger warning: purgeAge property is not supported. "
				          << "Use purgeCount instead to limit the number of rotated files." << std::endl;
			}

			RotationConfig rotConfig = parsePocoRotation(rotation, archive, purgeCount, rotateOnOpen);

			if (rotConfig.enabled)
			{
				info.sinks.push_back(createRotatingFileSink(filePath, rotConfig));
			}
			else
			{
				info.sinks.push_back(createFileSink(filePath));
			}
		}
		return;
	}

	// SimpleFileChannel - with rotation support
	if (SimpleFileChannel* pSimpleFileChannel = dynamic_cast<SimpleFileChannel*>(pChannel.get()))
	{
		std::string filePath = pSimpleFileChannel->path();
		if (!filePath.empty())
		{
			// Extract rotation property from SimpleFileChannel
			std::string rotation = pSimpleFileChannel->getProperty(SimpleFileChannel::PROP_ROTATION);

			// SimpleFileChannel only supports size-based rotation
			RotationConfig rotConfig = parsePocoRotation(rotation, "", "", false);

			if (rotConfig.enabled)
			{
				info.sinks.push_back(createRotatingFileSink(filePath, rotConfig));
			}
			else
			{
				info.sinks.push_back(createFileSink(filePath));
			}
		}
		return;
	}

	// StreamChannel - map to stdout stream sink
	// StreamChannel wraps an ostream, but we can't access it externally.
	// Default to stdout which is the most common use case.
	if (dynamic_cast<StreamChannel*>(pChannel.get()))
	{
		auto streamSink = PocoQuillFrontend::create_or_get_sink<quill::StreamSink>(
			"poco_stdout_stream",
			std::string("stdout")  // StreamSink takes fs::path, use "stdout" string
		);
		info.sinks.push_back(streamSink);
		return;
	}

#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_NO_SYSLOG_CHANNEL)
	// SyslogChannel
	if (SyslogChannel* pSyslogChannel = dynamic_cast<SyslogChannel*>(pChannel.get()))
	{
		std::string ident = pSyslogChannel->getProperty(SyslogChannel::PROP_NAME);
		int facility = LOG_USER;
		std::string facilityStr = pSyslogChannel->getProperty(SyslogChannel::PROP_FACILITY);
		if (!facilityStr.empty())
		{
			int facilityNum;
			if (Poco::NumberParser::tryParse(facilityStr, facilityNum))
			{
				facility = pocoToSyslogFacility(facilityNum);
			}
		}

		quill::SyslogSinkConfig cfg;
		cfg.set_identifier(ident.empty() ? "poco" : ident);
		cfg.set_facility(facility);

		auto syslogSink = PocoQuillFrontend::create_or_get_sink<quill::SyslogSink>(
			"poco_syslog_" + ident,
			cfg
		);
		info.sinks.push_back(syslogSink);
		return;
	}
#endif

#if defined(POCO_OS_FAMILY_WINDOWS)
	// WindowsConsoleChannel - use console sink
	if (dynamic_cast<WindowsConsoleChannel*>(pChannel.get()))
	{
		info.sinks.push_back(getConsoleSink());
		return;
	}

	// EventLogChannel - Windows Event Log
	// Quill doesn't have a native Windows Event Log sink, so we fall back to console
	// with a warning. Users requiring Event Log output should use the regular Logger.
	if (dynamic_cast<EventLogChannel*>(pChannel.get()))
	{
		// Fall back to console - Quill doesn't support Windows Event Log
		info.sinks.push_back(getConsoleSink());
		return;
	}
#endif

	// EventChannel - dispatches Poco Messages to in-process listeners. Quill
	// bypasses Poco's Message/Channel pipeline, so it cannot feed an EventChannel.
	// Skip it and warn, rather than add a spurious console sink via the fallback below.
	if (dynamic_cast<EventChannel*>(pChannel.get()))
	{
		std::cerr << "FastLogger warning: EventChannel is not supported by the fast "
		          << "logger; its output is omitted. Use a regular Logger for that "
		          << "channel if you need it." << std::endl;
		return;
	}

	// Unknown channel type - fall back to console
	info.sinks.push_back(getConsoleSink());
}

} // anonymous namespace


// Per-source routing state for when a FastLogger is installed as a Poco::Logger
// channel (the type=fast bridge). Retains the configured sinks and format so each
// distinct Message source routes to its own Quill logger (named after the source),
// preserving the %s/%(logger) field instead of collapsing every source to this
// logger's name. Held behind a void* in the header to keep it quill-free.
namespace {
struct PerSourceState
{
	std::vector<std::shared_ptr<quill::Sink>>          sinks;
	quill::PatternFormatterOptions                     options;
	std::unordered_map<std::string, PocoQuillLogger*>  loggers;
	Poco::FastMutex                                    mutex;
};

// Free per-source Quill loggers created under a previous configuration. Quill's
// frontend never releases loggers and keys them by name, so without removing the
// old ones, create_or_get_logger(source, ...) for an already-seen source returns
// the stale logger still bound to the old sinks, silently ignoring a reconfigure.
// remove_logger_blocking is Quill's supported way to recreate a logger of the same
// name with new sinks. It spin-waits, so call it outside state->mutex.
void releaseStaleSourceLoggers(std::unordered_map<std::string, PocoQuillLogger*>& stale)
{
	for (auto& entry: stale)
		PocoQuillFrontend::remove_logger_blocking(entry.second);
	stale.clear();
}
} // namespace


FastLogger::FastLogger(const std::string& name, int level):
	_name(name),
	_level(level),
	_pQuillLogger(nullptr),
	_pSourceState(nullptr)
{
	ensureBackendStarted();

	// Create the underlying Quill logger with the default sink
	PocoQuillLogger* logger = PocoQuillFrontend::create_or_get_logger(
		name.empty() ? "root" : name,
		getDefaultSink()
	);
	_pQuillLogger = static_cast<void*>(logger);

	// Quill stays permissive; the Poco logger's per-logger level is authoritative
	// (this channel does not re-gate). See log() and setLevel().
	logger->set_log_level(quill::LogLevel::TraceL3);
}


FastLogger::~FastLogger()
{
	// Quill manages logger lifetime; free our per-source routing state.
	delete static_cast<PerSourceState*>(_pSourceState.load());
}


void FastLogger::setLevel(int level)
{
	// _level gates the direct logger-API methods (trace()/debug()/...) and the
	// is()/trace() predicates. The Quill loggers stay permissive so the originating
	// Poco logger's level decides what the channel emits (see log() and
	// resolveSourceLogger).
	_level = level;
}


void FastLogger::setLevel(const std::string& level)
{
	setLevel(parseLevel(level));
}


void FastLogger::setChannel(Channel::Ptr pChannel)
{
	if (!pChannel) return;

	ensureBackendStarted();

	// Not thread-safe against concurrent log(): this reassigns _pQuillLogger and
	// rebuilds _pSourceState without a lock, while log()/resolveSourceLogger read
	// them. The contract is that logging is configured once at startup, before
	// logging begins; do not call setChannel() concurrently with logging.

	// Recursively collect all sinks from the channel hierarchy
	// This handles FormattingChannel, AsyncChannel, SplitterChannel, and all
	// supported channel types (Console, File, Syslog, Stream, etc.)
	// Also extracts PatternFormatter patterns if present.
	SinkInfo info;
	std::unordered_set<const Channel*> visited;
	collectSinksFromChannel(pChannel, info, visited);

	// Some sinks are process-wide singletons (e.g. the shared console sink), so a
	// channel tree can surface the same sink more than once; de-duplicate so Quill
	// does not emit each line multiple times.
	if (info.sinks.size() > 1)
	{
		std::vector<std::shared_ptr<quill::Sink>> uniqueSinks;
		for (auto& s: info.sinks)
		{
			bool dup = false;
			for (auto& u: uniqueSinks) { if (u == s) { dup = true; break; } }
			if (!dup) uniqueSinks.push_back(s);
		}
		info.sinks.swap(uniqueSinks);
	}

	if (!info.sinks.empty())
	{
		// Quill doesn't support changing sinks on an existing logger.
		// We create a new logger with a versioned name each time setChannel is called.
		// Always use a version suffix because the base name is already taken by the
		// constructor (which creates a logger with the default sink).
		// The version suffix must be unique for the process lifetime, not just this
		// FastLogger instance. Quill's frontend keeps previously created loggers
		// across shutdown(), so a per-instance counter would restart at 1 for a
		// recreated logger and collide with a stale Quill logger of the same name;
		// create_or_get_logger() would then return the old sinks and ignore the new
		// channel's. A process-wide counter guarantees a fresh logger every time.
		static std::atomic<Poco::UInt64> sLoggerVersion{0};
		const Poco::UInt64 version = ++sLoggerVersion;
		std::string loggerName = _name.empty() ? "root" : _name;
		loggerName += "_v";
		NumberFormatter::append(loggerName, version);

		// Create PatternFormatterOptions if a custom pattern was extracted
		quill::PatternFormatterOptions formatterOptions;
		if (!info.formatPattern.empty())
		{
			formatterOptions.format_pattern = info.formatPattern;
		}
		if (!info.timestampPattern.empty())
		{
			formatterOptions.timestamp_pattern = info.timestampPattern;
		}

		// Default to local time, set explicitly so it cannot regress if Quill's
		// default changes; a configured PatternFormatter "times"/%L overrides it
		// (see SinkInfo).
		formatterOptions.timestamp_timezone = info.timestampTimezone;

		// Retain the sinks and format so that, when this FastLogger is used as a
		// Poco::Logger channel (the bridge), each Message source routes to its own
		// Quill logger (named after the source) sharing them, preserving %s/%(logger)
		// instead of collapsing every source to this name.
		if (_pSourceState == nullptr) _pSourceState = new PerSourceState();
		{
			PerSourceState* state = static_cast<PerSourceState*>(_pSourceState.load());
			std::unordered_map<std::string, PocoQuillLogger*> stale;
			{
				Poco::FastMutex::ScopedLock lock(state->mutex);
				state->sinks = info.sinks;        // copy the shared sink pointers
				state->options = formatterOptions;
				stale.swap(state->loggers);       // detach per-source loggers from the prior config
			}
			releaseStaleSourceLoggers(stale);     // free them so the new sinks take effect
		}

		PocoQuillLogger* newLogger = PocoQuillFrontend::create_or_get_logger(
			loggerName,
			std::move(info.sinks),
			formatterOptions
		);
		newLogger->set_log_level(quill::LogLevel::TraceL3);  // permissive; the Poco logger level is authoritative
		_pQuillLogger = static_cast<void*>(newLogger);
	}
	else
	{
		// The channel tree yielded no Quill sink -- e.g. an EventChannel-only
		// configuration, which collectSinksFromChannel skips (no Quill analogue).
		// Leaving _pQuillLogger at the constructor's default console sink would emit
		// to the console despite an event-only configuration. Route to a null sink
		// instead, and mirror that into the per-source state.
		static std::atomic<Poco::UInt64> sNullLoggerVersion{0};
		const Poco::UInt64 version = ++sNullLoggerVersion;
		std::string loggerName = _name.empty() ? "root" : _name;
		loggerName += "_null_v";
		NumberFormatter::append(loggerName, version);

		std::vector<std::shared_ptr<quill::Sink>> nullSinks{ getNullSink() };
		if (_pSourceState == nullptr) _pSourceState = new PerSourceState();
		{
			PerSourceState* state = static_cast<PerSourceState*>(_pSourceState.load());
			std::unordered_map<std::string, PocoQuillLogger*> stale;
			{
				Poco::FastMutex::ScopedLock lock(state->mutex);
				state->sinks = nullSinks;
				state->options = quill::PatternFormatterOptions();
				stale.swap(state->loggers);
			}
			releaseStaleSourceLoggers(stale);
		}
		PocoQuillLogger* nullLogger = PocoQuillFrontend::create_or_get_logger(
			loggerName, std::move(nullSinks), quill::PatternFormatterOptions());
		nullLogger->set_log_level(quill::LogLevel::TraceL3);
		_pQuillLogger = static_cast<void*>(nullLogger);
	}
}


Channel::Ptr FastLogger::getChannel() const
{
	// FastLogger uses Quill's internal sinks, not Poco Channels.
	// Returns null for API compatibility.
	return nullptr;
}


void FastLogger::setProperty(const std::string& name, const std::string& value)
{
	if (name == "level")
	{
		setLevel(value);
	}
	else if (name == "channel")
	{
		// Look up channel by name in LoggingRegistry (same as Logger)
		setChannel(LoggingRegistry::defaultRegistry().channelForName(value));
	}
	else
	{
		Channel::setProperty(name, value);
	}
}


std::string FastLogger::getProperty(const std::string& name) const
{
	if (name == "level")
	{
		switch (_level)
		{
		case Message::PRIO_FATAL:       return "fatal";
		case Message::PRIO_CRITICAL:    return "critical";
		case Message::PRIO_ERROR:       return "error";
		case Message::PRIO_WARNING:     return "warning";
		case Message::PRIO_NOTICE:      return "notice";
		case Message::PRIO_INFORMATION: return "information";
		case Message::PRIO_DEBUG:       return "debug";
		case Message::PRIO_TRACE:       return "trace";
		default:                        return "none";
		}
	}
	else
	{
		return Channel::getProperty(name);
	}
}


void FastLogger::log(const Message& msg)
{
	// No level gate here. The originating Poco::Logger has already applied its
	// per-logger level (Logger::logImpl) before the message reaches this channel.
	// Re-gating on this channel's single _level would override per-logger levels
	// (a child set to trace would be dropped while root stays at information). In
	// Poco, loggers filter and channels do not; the Quill loggers stay permissive
	// (see the constructor, setChannel, resolveSourceLogger), so the Poco logger's
	// level is the single authority for what this channel emits.
	void* target = resolveSourceLogger(msg.getSource());
	const Priority prio = static_cast<Priority>(msg.getPriority());
	if (msg.getSourceFile() != nullptr)
	{
		std::string fullMsg;
		fullMsg.reserve(msg.getText().size() + 64);
		fullMsg.append(msg.getText());
		fullMsg.append(" [");
		fullMsg.append(msg.getSourceFile());
		fullMsg.append(":");
		NumberFormatter::append(fullMsg, static_cast<int>(msg.getSourceLine()));
		fullMsg.append("]");
		logToQuill(target, fullMsg, prio);
	}
	else
	{
		logToQuill(target, msg.getText(), prio);
	}
}


void FastLogger::log(const Exception& exc)
{
	error(exc.displayText());
}


void FastLogger::log(const Exception& exc, const char* file, LineNumber line)
{
	error(exc.displayText(), file, line);
}


void FastLogger::fatal(const std::string& msg)
{
	if (_level >= Priority::PRIO_FATAL)
		logImpl(msg, Priority::PRIO_FATAL);
}


void FastLogger::fatal(const std::string& msg, const char* file, LineNumber line)
{
	if (_level >= Priority::PRIO_FATAL)
		logImpl(msg, Priority::PRIO_FATAL, file, line);
}


void FastLogger::critical(const std::string& msg)
{
	if (_level >= Priority::PRIO_CRITICAL)
		logImpl(msg, Priority::PRIO_CRITICAL);
}


void FastLogger::critical(const std::string& msg, const char* file, LineNumber line)
{
	if (_level >= Priority::PRIO_CRITICAL)
		logImpl(msg, Priority::PRIO_CRITICAL, file, line);
}


void FastLogger::error(const std::string& msg)
{
	if (_level >= Priority::PRIO_ERROR)
		logImpl(msg, Priority::PRIO_ERROR);
}


void FastLogger::error(const std::string& msg, const char* file, LineNumber line)
{
	if (_level >= Priority::PRIO_ERROR)
		logImpl(msg, Priority::PRIO_ERROR, file, line);
}


void FastLogger::warning(const std::string& msg)
{
	if (_level >= Priority::PRIO_WARNING)
		logImpl(msg, Priority::PRIO_WARNING);
}


void FastLogger::warning(const std::string& msg, const char* file, LineNumber line)
{
	if (_level >= Priority::PRIO_WARNING)
		logImpl(msg, Priority::PRIO_WARNING, file, line);
}


void FastLogger::notice(const std::string& msg)
{
	if (_level >= Priority::PRIO_NOTICE)
		logImpl(msg, Priority::PRIO_NOTICE);
}


void FastLogger::notice(const std::string& msg, const char* file, LineNumber line)
{
	if (_level >= Priority::PRIO_NOTICE)
		logImpl(msg, Priority::PRIO_NOTICE, file, line);
}


void FastLogger::information(const std::string& msg)
{
	if (_level >= Priority::PRIO_INFORMATION)
		logImpl(msg, Priority::PRIO_INFORMATION);
}


void FastLogger::information(const std::string& msg, const char* file, LineNumber line)
{
	if (_level >= Priority::PRIO_INFORMATION)
		logImpl(msg, Priority::PRIO_INFORMATION, file, line);
}


void FastLogger::debug(const std::string& msg)
{
	if (_level >= Priority::PRIO_DEBUG)
		logImpl(msg, Priority::PRIO_DEBUG);
}


void FastLogger::debug(const std::string& msg, const char* file, LineNumber line)
{
	if (_level >= Priority::PRIO_DEBUG)
		logImpl(msg, Priority::PRIO_DEBUG, file, line);
}


void FastLogger::trace(const std::string& msg)
{
	if (_level >= Priority::PRIO_TRACE)
		logImpl(msg, Priority::PRIO_TRACE);
}


void FastLogger::trace(const std::string& msg, const char* file, LineNumber line)
{
	if (_level >= Priority::PRIO_TRACE)
		logImpl(msg, Priority::PRIO_TRACE, file, line);
}


void FastLogger::dump(const std::string& msg, const void* buffer, std::size_t length, Priority prio)
{
	if (_level >= prio)
	{
		std::string text(msg);
		formatDump(text, buffer, length);
		logImpl(text, prio);
	}
}


void FastLogger::logImpl(const std::string& text, Priority prio)
{
	logToQuill(_pQuillLogger, text, prio);
}


void* FastLogger::resolveSourceLogger(const std::string& source)
{
	PerSourceState* state = static_cast<PerSourceState*>(_pSourceState.load());
	if (state == nullptr || source.empty())
		return _pQuillLogger;

	Poco::FastMutex::ScopedLock lock(state->mutex);
	auto it = state->loggers.find(source);
	if (it != state->loggers.end())
		return it->second;

	// Bound growth: every distinct source permanently creates a Quill logger that
	// Quill's frontend never releases. Bridged logger names are expected to come
	// from a fixed set; if a caller routes high-cardinality dynamic names through
	// the bridge, cap the distinct per-source loggers and fall back to the base
	// logger for the overflow, losing only the %s/%(logger) attribution for those.
	static constexpr std::size_t kMaxSourceLoggers = 8192;
	if (state->loggers.size() >= kMaxSourceLoggers)
		return _pQuillLogger;

	// Lazily create a Quill logger named after the source, sharing the configured
	// sinks and format, so %(logger)/%s shows the originating logger name. The bare
	// source name is used for clean attribution. setChannel() removes the previous
	// configuration's per-source loggers on reconfigure (see releaseStaleSourceLoggers),
	// so create_or_get_logger here always builds against the current sinks. A given
	// source name is bridged by a single FastLogger (Poco's one-logger-per-name model).
	PocoQuillLogger* logger = PocoQuillFrontend::create_or_get_logger(
		source,
		state->sinks,
		state->options
	);
	logger->set_log_level(quill::LogLevel::TraceL3);  // permissive; the source Poco logger's level gates it
	state->loggers[source] = logger;
	return logger;
}


void FastLogger::logToQuill(void* pQuillLoggerVoid, const std::string& text, Priority prio)
{
	if (pQuillLoggerVoid == nullptr) return;

	PocoQuillLogger* logger = static_cast<PocoQuillLogger*>(pQuillLoggerVoid);

	// Use Quill's logging macros. We use QUILL_LOG_* prefixed macros
	// to avoid conflicts with syslog.h macros (LOG_INFO, LOG_WARNING, etc.)
	// when SyslogSink is enabled on Unix systems.
	switch (prio)
	{
	case Priority::PRIO_FATAL:
	case Priority::PRIO_CRITICAL:
		QUILL_LOG_CRITICAL(logger, "{}", text);
		break;
	case Priority::PRIO_ERROR:
		QUILL_LOG_ERROR(logger, "{}", text);
		break;
	case Priority::PRIO_WARNING:
		QUILL_LOG_WARNING(logger, "{}", text);
		break;
	case Priority::PRIO_NOTICE:
		QUILL_LOG_NOTICE(logger, "{}", text);
		break;
	case Priority::PRIO_INFORMATION:
		QUILL_LOG_INFO(logger, "{}", text);
		break;
	case Priority::PRIO_DEBUG:
		QUILL_LOG_DEBUG(logger, "{}", text);
		break;
	case Priority::PRIO_TRACE:
		QUILL_LOG_TRACE_L1(logger, "{}", text);
		break;
	default:
		QUILL_LOG_INFO(logger, "{}", text);
		break;
	}
}


void FastLogger::logImpl(const std::string& text, Priority prio, const char* file, LineNumber line)
{
	if (!_pQuillLogger) return;

	// Format message with source location
	std::string fullMsg;
	fullMsg.reserve(text.size() + 64);
	fullMsg.append(text);
	fullMsg.append(" [");
	fullMsg.append(file);
	fullMsg.append(":");
	NumberFormatter::append(fullMsg, static_cast<int>(line));
	fullMsg.append("]");

	logImpl(fullMsg, prio);
}


FastLogger& FastLogger::get(const std::string& name)
{
	Mutex::ScopedLock lock(_mapMtx);

	return unsafeGet(name);
}


FastLogger& FastLogger::unsafeGet(const std::string& name)
{
	Ptr pLogger = find(name);
	if (!pLogger)
	{
		if (name == ROOT)
		{
			pLogger = new FastLogger(name, Message::PRIO_INFORMATION);
		}
		else
		{
			FastLogger& parentLogger = parent(name);
			pLogger = new FastLogger(name, parentLogger.getLevel());
		}
		add(pLogger);
	}
	return *pLogger;
}


FastLogger& FastLogger::create(const std::string& name, int level)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (find(name))
		throw ExistsException("FastLogger already exists");

	Ptr pLogger = new FastLogger(name, level);
	add(pLogger);
	return *pLogger;
}


FastLogger& FastLogger::create(const std::string& name, Channel::Ptr pChannel, int level)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (find(name))
		throw ExistsException("FastLogger already exists");

	Ptr pLogger = new FastLogger(name, level);
	add(pLogger);

	// Apply the channel if provided
	if (pChannel)
	{
		pLogger->setChannel(pChannel);
	}

	return *pLogger;
}


FastLogger& FastLogger::root()
{
	return get(ROOT);
}


FastLogger::Ptr FastLogger::has(const std::string& name)
{
	Mutex::ScopedLock lock(_mapMtx);
	return find(name);
}


void FastLogger::destroy(const std::string& name)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (_pLoggerMap)
	{
		auto it = _pLoggerMap->find(name);
		if (it != _pLoggerMap->end())
		{
			_pLoggerMap->erase(it);
		}
	}
}


void FastLogger::shutdown()
{
	Mutex::ScopedLock lock(_mapMtx);

	if (_pLoggerMap)
	{
		_pLoggerMap->clear();
	}

	// Stop the Quill backend (flushes all pending messages)
	{
		Mutex::ScopedLock backendLock(_backendMtx);
		if (_backendStarted)
		{
			quill::Backend::stop();
			_backendStarted = false;
		}
	}

	// Clear pending backend options so they can be set again after restart
	{
		Mutex::ScopedLock optionsLock(_backendOptionsMtx);
		_pendingBackendOptions.clear();
	}
}


void FastLogger::names(std::vector<std::string>& names)
{
	Mutex::ScopedLock lock(_mapMtx);

	names.clear();
	if (_pLoggerMap)
	{
		for (const auto& p : *_pLoggerMap)
		{
			names.push_back(p.first);
		}
	}
}


int FastLogger::parseLevel(const std::string& level)
{
	// Same parsing as Logger::parseLevel
	if (icompare(level, "none") == 0)
		return 0;
	else if (icompare(level, "fatal") == 0)
		return Message::PRIO_FATAL;
	else if (icompare(level, "critical") == 0)
		return Message::PRIO_CRITICAL;
	else if (icompare(level, "error") == 0)
		return Message::PRIO_ERROR;
	else if (icompare(level, "warning") == 0)
		return Message::PRIO_WARNING;
	else if (icompare(level, "notice") == 0)
		return Message::PRIO_NOTICE;
	else if (icompare(level, "information") == 0)
		return Message::PRIO_INFORMATION;
	else if (icompare(level, "debug") == 0)
		return Message::PRIO_DEBUG;
	else if (icompare(level, "trace") == 0)
		return Message::PRIO_TRACE;
	else
	{
		int numLevel;
		if (Poco::NumberParser::tryParse(level, numLevel))
		{
			if (numLevel > 0 && numLevel < 9)
				return numLevel;
		}
		throw InvalidArgumentException("Unknown log level", level);
	}
}


void FastLogger::setLevel(const std::string& name, int level)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (_pLoggerMap)
	{
		std::string prefix = name;
		if (!prefix.empty()) prefix += '.';

		for (auto& p : *_pLoggerMap)
		{
			if (p.first == name || p.first.compare(0, prefix.size(), prefix) == 0)
			{
				p.second->setLevel(level);
			}
		}
	}
}


void FastLogger::setChannel(const std::string& name, Channel::Ptr pChannel)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (_pLoggerMap)
	{
		std::string prefix = name;
		if (!prefix.empty()) prefix += '.';

		for (auto& p : *_pLoggerMap)
		{
			if (p.first == name || p.first.compare(0, prefix.size(), prefix) == 0)
			{
				p.second->setChannel(pChannel);
			}
		}
	}
}


void FastLogger::setProperty(const std::string& loggerName, const std::string& propertyName, const std::string& value)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (_pLoggerMap)
	{
		std::string prefix = loggerName;
		if (!prefix.empty()) prefix += '.';

		for (auto& p : *_pLoggerMap)
		{
			if (p.first == loggerName || p.first.compare(0, prefix.size(), prefix) == 0)
			{
				p.second->setProperty(propertyName, value);
			}
		}
	}
}


std::string FastLogger::formatImpl(const std::string& fmt, int argc, std::string argv[])
{
	std::string result;
	result.reserve(fmt.size() + 64);
	std::string::const_iterator it = fmt.begin();
	while (it != fmt.end())
	{
		if (*it == '$')
		{
			++it;
			if (it != fmt.end() && *it >= '0' && *it <= '9')
			{
				int i = *it - '0';
				if (i < argc)
					result.append(argv[i]);
				++it;
			}
			else if (it != fmt.end() && *it == '$')
			{
				result += '$';
				++it;
			}
			else
			{
				result += '$';
			}
		}
		else
		{
			result += *it++;
		}
	}
	return result;
}


std::string FastLogger::format(const std::string& fmt, const std::string& arg)
{
	std::string args[] = { arg };
	return formatImpl(fmt, 1, args);
}


std::string FastLogger::format(const std::string& fmt, const std::string& arg0, const std::string& arg1)
{
	std::string args[] = { arg0, arg1 };
	return formatImpl(fmt, 2, args);
}


std::string FastLogger::format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2)
{
	std::string args[] = { arg0, arg1, arg2 };
	return formatImpl(fmt, 3, args);
}


std::string FastLogger::format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2, const std::string& arg3)
{
	std::string args[] = { arg0, arg1, arg2, arg3 };
	return formatImpl(fmt, 4, args);
}


void FastLogger::formatDump(std::string& message, const void* buffer, std::size_t length)
{
	const int BYTES_PER_LINE = 16;
	const unsigned char* base = reinterpret_cast<const unsigned char*>(buffer);
	message.reserve(message.size() + length * 6);
	if (!message.empty()) message.append("\n");

	for (std::size_t i = 0; i < length; i += BYTES_PER_LINE)
	{
		// Offset
		NumberFormatter::appendHex(message, static_cast<unsigned>(i), 4);
		message.append("  ");

		// Hex bytes
		for (int j = 0; j < BYTES_PER_LINE; ++j)
		{
			if (i + j < length)
			{
				NumberFormatter::appendHex(message, base[i + j], 2);
				message.append(" ");
			}
			else
			{
				message.append("   ");
			}
		}
		message.append(" ");

		// ASCII representation
		for (int j = 0; j < BYTES_PER_LINE && i + j < length; ++j)
		{
			unsigned char c = base[i + j];
			if (c >= 32 && c < 127)
				message += static_cast<char>(c);
			else
				message += '.';
		}
		message.append("\n");
	}
}


void FastLogger::startBackend()
{
	ensureBackendStarted();
}


void FastLogger::flush()
{
	if (_pQuillLogger)
	{
		static_cast<PocoQuillLogger*>(_pQuillLogger.load())->flush_log();
	}

	// Bridge path: messages routed through resolveSourceLogger() land in per-source
	// Quill loggers, not _pQuillLogger, so drain those too. Otherwise flush() on its
	// own (without a following shutdown(), whose Backend::stop() flushes everything)
	// leaves bridged output queued in the backend. Use the same state mutex
	// resolveSourceLogger takes to create them.
	if (PerSourceState* state = static_cast<PerSourceState*>(_pSourceState.load()))
	{
		Poco::FastMutex::ScopedLock lock(state->mutex);
		for (auto& kv: state->loggers)
			if (kv.second) kv.second->flush_log();
	}
}


void FastLogger::setPattern(const std::string& /* pattern */)
{
	// Pattern and sinks are configured per logger through the Channel passed to
	// setChannel() (a PatternFormatter on a FormattingChannel). Each Quill logger
	// binds its sinks and format at creation and cannot be mutated afterwards, so a
	// global setPattern cannot be honored; throw rather than silently ignore it.
	throw NotImplementedException(
		"FastLogger::setPattern: configure the pattern via a PatternFormatter on the "
		"FormattingChannel passed to setChannel() (logging.loggers...type=fast)");
}


void FastLogger::addFileSink(const std::string& /* filename */)
{
	// Sinks are derived from the Channel passed to setChannel() (e.g. a FileChannel,
	// possibly inside a SplitterChannel); there is no global sink list to add to in
	// the per-source bridge model. Throw rather than create a sink that is silently
	// dropped, which would also truncate the target file as a side effect.
	throw NotImplementedException(
		"FastLogger::addFileSink: configure file output via a FileChannel passed to "
		"setChannel() (logging.loggers...type=fast)");
}


void FastLogger::setBackendOption(const std::string& name, const std::string& value)
{
	// Lock order: _backendMtx first, then _backendOptionsMtx
	// This matches the order in ensureBackendStarted()
	Mutex::ScopedLock backendLock(_backendMtx);

	if (_backendStarted)
	{
		throw IllegalStateException("Cannot set backend option after logging has started: " + name);
	}

	Mutex::ScopedLock optionsLock(_backendOptionsMtx);
	_pendingBackendOptions[name] = value;
}


FastLogger& FastLogger::parent(const std::string& name)
{
	std::string::size_type pos = name.rfind('.');
	if (pos != std::string::npos)
	{
		std::string parentName = name.substr(0, pos);
		Ptr pParent = find(parentName);
		if (pParent)
			return *pParent;
		else
			return parent(parentName);
	}
	else
	{
		return unsafeGet(ROOT);
	}
}


void FastLogger::add(Ptr pLogger)
{
	if (!_pLoggerMap)
	{
		_pLoggerMap = std::make_unique<LoggerMap>();
	}
	_pLoggerMap->insert(std::make_pair(pLogger->name(), pLogger));
}


FastLogger::Ptr FastLogger::find(const std::string& name)
{
	if (_pLoggerMap)
	{
		auto it = _pLoggerMap->find(name);
		if (it != _pLoggerMap->end())
			return it->second;
	}
	return nullptr;
}


void FastLogger::ensureBackendStarted()
{
	Mutex::ScopedLock lock(_backendMtx);
	if (!_backendStarted)
	{
		quill::BackendOptions options;

		// No sleep - backend should spin when there's work to do
		options.sleep_duration = std::chrono::nanoseconds{0};

		// Enable yielding when idle for better CPU sharing
		options.enable_yield_when_idle = true;

		// Disable timestamp ordering grace period for minimum latency
		options.log_timestamp_ordering_grace_period = std::chrono::microseconds{0};

		// Minimize flush interval for faster throughput (default is 200ms)
		options.sink_min_flush_interval = std::chrono::milliseconds{0};

		// Use a no-op error notifier to suppress queue reallocation messages
		options.error_notifier = [](std::string const&) { /* suppress */ };

#if defined(__linux__) || defined(_WIN32)
		// Check if CPU affinity is enabled (default: false, same as Quill)
		// Can be set via setBackendOption("enableCpuAffinity", "true") before first logger creation
		bool enableCpuAffinity = false;
		{
			Mutex::ScopedLock optionsLock(_backendOptionsMtx);
			auto it = std::find_if(_pendingBackendOptions.begin(), _pendingBackendOptions.end(),
				[](const auto& opt) { return opt.first == "enableCpuAffinity"; });
			if (it != _pendingBackendOptions.end())
			{
				enableCpuAffinity = (icompare(it->second, "true") == 0 || it->second == "1");
			}
		}

#if defined(__linux__)
		// On Linux, pin the backend thread to the last CPU core to reduce
		// contention with application threads
		if (enableCpuAffinity)
		{
			long num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
			if (num_cpus > 1)
			{
				options.cpu_affinity = static_cast<uint16_t>(num_cpus - 1);
			}
		}
#elif defined(_WIN32)
		// On Windows, pin the backend thread to the last CPU core
		if (enableCpuAffinity)
		{
			SYSTEM_INFO sysInfo;
			GetSystemInfo(&sysInfo);
			DWORD num_cpus = sysInfo.dwNumberOfProcessors;
			if (num_cpus > 1)
			{
				options.cpu_affinity = static_cast<uint16_t>(num_cpus - 1);
			}
		}
#endif
#endif // __linux__ || _WIN32

		// Apply any pending backend options
		{
			Mutex::ScopedLock optionsLock(_backendOptionsMtx);
			// On a malformed value, warn and skip rather than throw.
			// ensureBackendStarted() runs lazily on the first logger or log call, so a
			// throw here would surface from an unrelated get()/log() call far from the
			// setBackendOption() site.
			auto parseU64 = [](const std::string& n, const std::string& v, Poco::UInt64& out) -> bool
			{
				if (NumberParser::tryParseUnsigned64(v, out)) return true;
				std::cerr << "FastLogger warning: ignoring non-numeric value \"" << v
				          << "\" for backend option \"" << n << "\"." << std::endl;
				return false;
			};

			for (const auto& opt : _pendingBackendOptions)
			{
				const std::string& name = opt.first;
				const std::string& value = opt.second;
				Poco::UInt64 num = 0;

				if (name == "threadName")
				{
					options.thread_name = value;
				}
				else if (name == "sleepDuration")
				{
					if (parseU64(name, value, num))
						options.sleep_duration = std::chrono::microseconds{num};
				}
				else if (name == "enableYieldWhenIdle")
				{
					options.enable_yield_when_idle = (icompare(value, "true") == 0 || value == "1");
				}
				else if (name == "cpuAffinity")
				{
					if (parseU64(name, value, num))
						options.cpu_affinity = static_cast<uint16_t>(num);
				}
				else if (name == "transitEventBufferInitialCapacity")
				{
					if (parseU64(name, value, num))
						options.transit_event_buffer_initial_capacity = static_cast<size_t>(num);
				}
				else if (name == "transitEventsSoftLimit")
				{
					if (parseU64(name, value, num))
						options.transit_events_soft_limit = static_cast<size_t>(num);
				}
				else if (name == "transitEventsHardLimit")
				{
					if (parseU64(name, value, num))
						options.transit_events_hard_limit = static_cast<size_t>(num);
				}
				else if (name == "logTimestampOrderingGracePeriod")
				{
					if (parseU64(name, value, num))
						options.log_timestamp_ordering_grace_period = std::chrono::microseconds{num};
				}
				else if (name == "sinkMinFlushInterval")
				{
					if (parseU64(name, value, num))
						options.sink_min_flush_interval = std::chrono::milliseconds{num};
				}
				else if (name == "waitForQueuesToEmptyBeforeExit")
				{
					options.wait_for_queues_to_empty_before_exit = (icompare(value, "true") == 0 || value == "1");
				}
				else if (name == "enableCpuAffinity")
				{
					// Already processed above, skip here
				}
				// Unknown options are silently ignored
			}
		}

		quill::Backend::start(options);
		_backendStarted = true;
	}
}


} // namespace Poco


#endif // POCO_ENABLE_FASTLOGGER
