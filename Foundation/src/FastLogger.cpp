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
	Mutex g_backendOptionsMtx;
}


namespace {

// Map Poco log levels to Quill log levels
quill::LogLevel pocoToQuillLevel(int pocoLevel)
{
	switch (pocoLevel)
	{
	case Message::PRIO_FATAL:
		return quill::LogLevel::Critical;  // Quill doesn't have Fatal, use Critical
	case Message::PRIO_CRITICAL:
		return quill::LogLevel::Critical;
	case Message::PRIO_ERROR:
		return quill::LogLevel::Error;
	case Message::PRIO_WARNING:
		return quill::LogLevel::Warning;
	case Message::PRIO_NOTICE:
		return quill::LogLevel::Notice;
	case Message::PRIO_INFORMATION:
		return quill::LogLevel::Info;
	case Message::PRIO_DEBUG:
		return quill::LogLevel::Debug;
	case Message::PRIO_TRACE:
		return quill::LogLevel::TraceL1;
	default:
		if (pocoLevel <= 0)
			return quill::LogLevel::None;
		return quill::LogLevel::Info;
	}
}


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


// Translate Poco PatternFormatter pattern to Quill format pattern
// Returns a pair of (format_pattern, timestamp_pattern)
std::pair<std::string, std::string> translatePocoPatternToQuill(const std::string& pocoPattern)
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

	std::string quillPattern;
	std::string timestampPattern;
	bool hasTimestamp = false;

	quillPattern.reserve(pocoPattern.size() * 2);
	timestampPattern.reserve(64);

	std::string::const_iterator it = pocoPattern.begin();
	std::string::const_iterator end = pocoPattern.end();

	while (it != end)
	{
		if (*it == '%')
		{
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
			// Message content
			case 't':  // message text
				quillPattern += "%(message)";
				break;
			case 's':  // message source (logger name)
				quillPattern += "%(logger)";
				break;

			// Log level
			case 'l':  // level number - Quill doesn't have this, use short code
				quillPattern += "%(log_level_short_code)";
				break;
			case 'p':  // priority name (Fatal, Critical, etc.)
				quillPattern += "%(log_level)";
				break;
			case 'q':  // abbreviated priority (F, C, E, W, N, I, D, T)
				quillPattern += "%(log_level_short_code)";
				break;

			// Process/Thread info
			case 'P':  // process id
				quillPattern += "%(process_id)";
				break;
			case 'T':  // thread name
				quillPattern += "%(thread_name)";
				break;
			case 'I':  // thread id
				quillPattern += "%(thread_id)";
				break;
			case 'J':  // OS thread id - use thread_id as fallback
				quillPattern += "%(thread_id)";
				break;
			case 'N':  // hostname - hardcoded from cached value
				quillPattern += getCachedHostname();
				break;

			// Source location
			case 'U':  // source file path
				quillPattern += "%(full_path)";
				break;
			case 'O':  // source file basename
				quillPattern += "%(file_name)";
				break;
			case 'u':  // source line number
				quillPattern += "%(line_number)";
				break;

			// Date/time specifiers - collect into timestamp pattern
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
				if (!hasTimestamp)
				{
					quillPattern += "%(time)";
					hasTimestamp = true;
				}
				// Build timestamp pattern
				timestampPattern += '%';
				if (spec == 'n' || spec == 'e')
					timestampPattern += (spec == 'n') ? 'm' : 'd';  // Quill uses standard strftime
				else
					timestampPattern += spec;
				break;

			case 'i':  // milliseconds
				if (!hasTimestamp)
				{
					quillPattern += "%(time)";
					hasTimestamp = true;
				}
				timestampPattern += "%Qms";
				break;

			case 'c':  // centiseconds - approximate with milliseconds
				if (!hasTimestamp)
				{
					quillPattern += "%(time)";
					hasTimestamp = true;
				}
				timestampPattern += "%Qms";
				break;

			case 'F':  // microseconds
				if (!hasTimestamp)
				{
					quillPattern += "%(time)";
					hasTimestamp = true;
				}
				timestampPattern += "%Qus";
				break;

			case 'E':  // epoch time - not directly supported
				quillPattern += "%(time)";
				hasTimestamp = true;
				break;

			case 'L':  // local time flag - affects timestamp handling
				// Quill handles timezone separately, skip
				break;

			case 'v':  // source with width - just use logger name
				// Skip optional width specifier [n]
				if (it != end && *it == '[')
				{
					while (it != end && *it != ']') ++it;
					if (it != end) ++it;
				}
				quillPattern += "%(logger)";
				break;

			case '[':  // custom property %[name] - not supported in Quill
				{
					// Skip until ]
					while (it != end && *it != ']') ++it;
					if (it != end) ++it;
					quillPattern += "[prop]";
				}
				break;

			case '%':  // literal %
				quillPattern += '%';
				break;

			default:
				// Unknown specifier, pass through
				quillPattern += '%';
				quillPattern += spec;
				break;
			}
		}
		else
		{
			quillPattern += *it;
			++it;
		}
	}

	// If no timestamp pattern was built, use a sensible default
	if (timestampPattern.empty() && hasTimestamp)
	{
		timestampPattern = "%Y-%m-%d %H:%M:%S.%Qms";
	}
	else if (timestampPattern.empty())
	{
		timestampPattern = "%H:%M:%S.%Qns";  // Quill default
	}

	return std::make_pair(quillPattern, timestampPattern);
}


// Structure to hold collected sink info including formatter
struct SinkInfo
{
	std::vector<std::shared_ptr<quill::Sink>> sinks;
	std::string formatPattern;      // Quill format pattern (empty = default)
	std::string timestampPattern;   // Quill timestamp pattern (empty = default)
};


// Forward declaration for recursive sink collection
void collectSinksFromChannel(Channel::Ptr pChannel, SinkInfo& info);


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
void collectSinksFromChannel(Channel::Ptr pChannel, SinkInfo& info)
{
	if (!pChannel) return;

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
						info.formatPattern = translated.first;
						info.timestampPattern = translated.second;
					}
				}
			}
		}
		collectSinksFromChannel(pFormattingChannel->getChannel(), info);
		return;
	}

	// AsyncChannel - FastLogger is already async, just forward to inner channel
	if (AsyncChannel* pAsyncChannel = dynamic_cast<AsyncChannel*>(pChannel.get()))
	{
		collectSinksFromChannel(pAsyncChannel->getChannel(), info);
		return;
	}

	// SplitterChannel - the internal channel list is not accessible, so we fall back to console.
	// Users needing multiple sinks should configure them directly with FastLogger::addFileSink()
	// or by creating multiple FastLoggers.
	if (dynamic_cast<SplitterChannel*>(pChannel.get()))
	{
		// SplitterChannel's _channels vector is private with no accessor.
		// Fall back to console as a reasonable default.
		info.sinks.push_back(getConsoleSink());
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

	// Unknown channel type - fall back to console
	info.sinks.push_back(getConsoleSink());
}

} // anonymous namespace


FastLogger::FastLogger(const std::string& name, int level):
	_name(name),
	_level(level),
	_pQuillLogger(nullptr),
	_sinkVersion(0)
{
	ensureBackendStarted();

	// Create the underlying Quill logger with the default sink
	PocoQuillLogger* logger = PocoQuillFrontend::create_or_get_logger(
		name.empty() ? "root" : name,
		getDefaultSink()
	);
	_pQuillLogger = static_cast<void*>(logger);

	// Set the log level
	logger->set_log_level(pocoToQuillLevel(level));
}


FastLogger::~FastLogger()
{
	// Quill manages logger lifetime
}


void FastLogger::setLevel(int level)
{
	_level = level;
	if (_pQuillLogger)
	{
		static_cast<PocoQuillLogger*>(_pQuillLogger)->set_log_level(pocoToQuillLevel(level));
	}
}


void FastLogger::setLevel(const std::string& level)
{
	setLevel(parseLevel(level));
}


void FastLogger::setChannel(Channel::Ptr pChannel)
{
	if (!pChannel) return;

	ensureBackendStarted();

	// Recursively collect all sinks from the channel hierarchy
	// This handles FormattingChannel, AsyncChannel, SplitterChannel, and all
	// supported channel types (Console, File, Syslog, Stream, etc.)
	// Also extracts PatternFormatter patterns if present.
	SinkInfo info;
	collectSinksFromChannel(pChannel, info);

	if (!info.sinks.empty())
	{
		// Quill doesn't support changing sinks on an existing logger.
		// We create a new logger with a versioned name each time setChannel is called.
		// Always use a version suffix because the base name is already taken by the
		// constructor (which creates a logger with the default sink).
		++_sinkVersion;
		std::string loggerName = _name.empty() ? "root" : _name;
		loggerName += "_v";
		NumberFormatter::append(loggerName, _sinkVersion);

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

		PocoQuillLogger* newLogger = PocoQuillFrontend::create_or_get_logger(
			loggerName,
			std::move(info.sinks),
			formatterOptions
		);
		newLogger->set_log_level(pocoToQuillLevel(_level));
		_pQuillLogger = static_cast<void*>(newLogger);
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
	if (_level >= msg.getPriority())
	{
		if (msg.getSourceFile())
			logImpl(msg.getText(), static_cast<Priority>(msg.getPriority()), msg.getSourceFile(), msg.getSourceLine());
		else
			logImpl(msg.getText(), static_cast<Priority>(msg.getPriority()));
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
	if (!_pQuillLogger) return;

	PocoQuillLogger* logger = static_cast<PocoQuillLogger*>(_pQuillLogger);

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
		Mutex::ScopedLock optionsLock(g_backendOptionsMtx);
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
		static_cast<PocoQuillLogger*>(_pQuillLogger)->flush_log();
	}
}


void FastLogger::setPattern(const std::string& pattern)
{
	// Quill uses different pattern syntax, store for new loggers
	// This would need to be applied when creating sinks
	// For now, we use Quill's default pattern
}


void FastLogger::addFileSink(const std::string& filename)
{
	ensureBackendStarted();

	auto fileSink = PocoQuillFrontend::create_or_get_sink<quill::FileSink>(
		filename,
		[]() {
			quill::FileSinkConfig cfg;
			cfg.set_open_mode('w');
			return cfg;
		}(),
		quill::FileEventNotifier{}
	);

	// Add to existing loggers - this would need more sophisticated handling
	// for production use (e.g., maintaining list of sinks per logger)
}


void FastLogger::setBackendOption(const std::string& name, const std::string& value)
{
	Mutex::ScopedLock lock(g_backendOptionsMtx);

	// Check if backend is already started
	{
		Mutex::ScopedLock backendLock(_backendMtx);
		if (_backendStarted)
		{
			throw IllegalStateException("Cannot set backend option after logging has started: " + name);
		}
	}

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

		// Check if CPU affinity is enabled (default: false, same as Quill)
		bool enableCpuAffinity = false;
		{
			Mutex::ScopedLock optionsLock(g_backendOptionsMtx);
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

		// Apply any pending backend options
		{
			Mutex::ScopedLock optionsLock(g_backendOptionsMtx);
			for (const auto& opt : _pendingBackendOptions)
			{
				const std::string& name = opt.first;
				const std::string& value = opt.second;

				if (name == "threadName")
				{
					options.thread_name = value;
				}
				else if (name == "sleepDuration")
				{
					options.sleep_duration = std::chrono::microseconds{NumberParser::parseUnsigned64(value)};
				}
				else if (name == "enableYieldWhenIdle")
				{
					options.enable_yield_when_idle = (icompare(value, "true") == 0 || value == "1");
				}
				else if (name == "cpuAffinity")
				{
					options.cpu_affinity = static_cast<uint16_t>(NumberParser::parseUnsigned(value));
				}
				else if (name == "transitEventBufferInitialCapacity")
				{
					options.transit_event_buffer_initial_capacity = static_cast<size_t>(NumberParser::parseUnsigned64(value));
				}
				else if (name == "transitEventsSoftLimit")
				{
					options.transit_events_soft_limit = static_cast<size_t>(NumberParser::parseUnsigned64(value));
				}
				else if (name == "transitEventsHardLimit")
				{
					options.transit_events_hard_limit = static_cast<size_t>(NumberParser::parseUnsigned64(value));
				}
				else if (name == "logTimestampOrderingGracePeriod")
				{
					options.log_timestamp_ordering_grace_period = std::chrono::microseconds{NumberParser::parseUnsigned64(value)};
				}
				else if (name == "sinkMinFlushInterval")
				{
					options.sink_min_flush_interval = std::chrono::milliseconds{NumberParser::parseUnsigned64(value)};
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
