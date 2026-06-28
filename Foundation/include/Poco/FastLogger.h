//
// FastLogger.h
//
// Library: Foundation
// Package: Logging
// Module:  FastLogger
//
// Definition of the FastLogger class.
//
// Copyright (c) 2004-2024, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FastLogger_INCLUDED
#define Foundation_FastLogger_INCLUDED


#include "Poco/Foundation.h"


#ifdef POCO_ENABLE_FASTLOGGER
#include "Poco/Message.h"
#include "Poco/Channel.h"
#include "Poco/Format.h"
#include "Poco/Mutex.h"
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <cstddef>


namespace Poco {


class Exception;


class Foundation_API FastLogger: public Channel
	/// FastLogger is a high-performance drop-in replacement for Poco::Logger
	/// that uses the Quill logging library internally.
	///
	/// FastLogger provides the same API as Logger but achieves much lower
	/// latency (~9ns vs ~100ns) by using asynchronous logging with lock-free
	/// queues and a dedicated backend thread for I/O.
	///
	/// Key differences from Logger:
	///   - Logging is asynchronous - messages are queued and processed by a
	///     background thread, providing very low latency for the caller.
	///   - Formatting and output are handled by Quill's sinks internally,
	///     though setChannel() maps Poco channels to Quill sinks.
	///   - Thread-safe by design without mutex locks in the hot path.
	///
	/// FastLogger instances are organized hierarchically by name, similar to
	/// Logger. The name uses dot-separated components (e.g., "HTTP.Server.Request").
	///
	/// Usage is identical to Logger:
	///     FastLogger& logger = FastLogger::get("MyApp.Component");
	///     logger.information("Processing request");
	///     logger.error("Failed to connect: %s", errorMsg);
	///
	/// Configuration via LoggingConfigurator is supported using the
	/// "fastloggers" section instead of "loggers".
{
public:
	using Ptr = AutoPtr<FastLogger>;
	using Priority = Message::Priority;

	[[nodiscard]]
	const std::string& name() const;
		/// Returns the name of the logger.

	void setChannel(Channel::Ptr pChannel);
		/// Sets the Channel. For FastLogger this is a no-op as Quill
		/// handles output internally, but provided for API compatibility.

	[[nodiscard]]
	Channel::Ptr getChannel() const;
		/// Returns the Channel. For FastLogger this returns a null pointer
		/// as Quill handles output internally, but provided for API compatibility.

	void setLevel(int level);
		/// Sets the logger's log level.
		///
		/// See Message::Priority for valid log levels.
		/// Setting the log level to zero turns off
		/// logging for that logger.

	[[nodiscard]]
	int getLevel() const;
		/// Returns the logger's log level.

	void setLevel(const std::string& level);
		/// Sets the logger's log level using a symbolic value.
		///
		/// Valid values are:
		///   - none (turns off logging)
		///   - fatal
		///   - critical
		///   - error
		///   - warning
		///   - notice
		///   - information
		///   - debug
		///   - trace

	void setProperty(const std::string& name, const std::string& value) override;
		/// Sets a configuration property.
		///
		/// Supported properties:
		///   - channel: Sets the channel via LoggingRegistry
		///   - level: Sets the log level

	[[nodiscard]]
	std::string getProperty(const std::string& name) const override;
		/// Returns the value of the property with the given name.
		///
		/// Supported properties:
		///   - level: Returns the log level as a string

	void log(const Message& msg) override;
		/// Logs the given message if its priority is
		/// greater than or equal to the logger's log level.

	// Bring log(Message&&) into scope; otherwise log(const Exception&) hides it
	using Channel::log;

	void log(const Exception& exc);
		/// Logs the given exception with priority PRIO_ERROR.

	void log(const Exception& exc, const char* file, LineNumber line);
		/// Logs the given exception with priority PRIO_ERROR.
		///
		/// File must be a static string, such as the value of
		/// the __FILE__ macro.

	void fatal(const std::string& msg);
		/// Logs a fatal message.

	void fatal(const std::string& msg, const char* file, LineNumber line);
		/// Logs a fatal message with source location.

	template <typename T, typename... Args>
	void fatal(const std::string& fmt, T arg1, Args&&... args)
	{
		if (fatal())
			logImpl(Poco::format(fmt, arg1, std::forward<Args>(args)...), Priority::PRIO_FATAL);
	}

	void critical(const std::string& msg);
		/// Logs a critical message.

	void critical(const std::string& msg, const char* file, LineNumber line);
		/// Logs a critical message with source location.

	template <typename T, typename... Args>
	void critical(const std::string& fmt, T arg1, Args&&... args)
	{
		if (critical())
			logImpl(Poco::format(fmt, arg1, std::forward<Args>(args)...), Priority::PRIO_CRITICAL);
	}

	void error(const std::string& msg);
		/// Logs an error message.

	void error(const std::string& msg, const char* file, LineNumber line);
		/// Logs an error message with source location.

	template <typename T, typename... Args>
	void error(const std::string& fmt, T arg1, Args&&... args)
	{
		if (error())
			logImpl(Poco::format(fmt, arg1, std::forward<Args>(args)...), Priority::PRIO_ERROR);
	}

	void warning(const std::string& msg);
		/// Logs a warning message.

	void warning(const std::string& msg, const char* file, LineNumber line);
		/// Logs a warning message with source location.

	template <typename T, typename... Args>
	void warning(const std::string& fmt, T arg1, Args&&... args)
	{
		if (warning())
			logImpl(Poco::format(fmt, arg1, std::forward<Args>(args)...), Priority::PRIO_WARNING);
	}

	void notice(const std::string& msg);
		/// Logs a notice message.

	void notice(const std::string& msg, const char* file, LineNumber line);
		/// Logs a notice message with source location.

	template <typename T, typename... Args>
	void notice(const std::string& fmt, T arg1, Args&&... args)
	{
		if (notice())
			logImpl(Poco::format(fmt, arg1, std::forward<Args>(args)...), Priority::PRIO_NOTICE);
	}

	void information(const std::string& msg);
		/// Logs an informational message.

	void information(const std::string& msg, const char* file, LineNumber line);
		/// Logs an informational message with source location.

	template <typename T, typename... Args>
	void information(const std::string& fmt, T arg1, Args&&... args)
	{
		if (information())
			logImpl(Poco::format(fmt, arg1, std::forward<Args>(args)...), Priority::PRIO_INFORMATION);
	}

	void debug(const std::string& msg);
		/// Logs a debug message.

	void debug(const std::string& msg, const char* file, LineNumber line);
		/// Logs a debug message with source location.

	template <typename T, typename... Args>
	void debug(const std::string& fmt, T arg1, Args&&... args)
	{
		if (debug())
			logImpl(Poco::format(fmt, arg1, std::forward<Args>(args)...), Priority::PRIO_DEBUG);
	}

	void trace(const std::string& msg);
		/// Logs a trace message.

	void trace(const std::string& msg, const char* file, LineNumber line);
		/// Logs a trace message with source location.

	template <typename T, typename... Args>
	void trace(const std::string& fmt, T arg1, Args&&... args)
	{
		if (trace())
			logImpl(Poco::format(fmt, arg1, std::forward<Args>(args)...), Priority::PRIO_TRACE);
	}

	void dump(const std::string& msg, const void* buffer, std::size_t length, Priority prio = Priority::PRIO_DEBUG);
		/// Logs the given message, followed by the data in buffer.
		///
		/// The data in buffer is written in canonical hex+ASCII form:
		/// Offset (4 bytes) in hexadecimal, followed by sixteen
		/// space-separated, two column, hexadecimal bytes,
		/// followed by the same sixteen bytes as ASCII characters.
		/// For bytes outside the range 32 .. 127, a dot is printed.

	[[nodiscard]]
	bool is(int level) const;
		/// Returns true if at least the given log level is set.

	[[nodiscard]]
	bool fatal() const;
		/// Returns true if the log level is at least PRIO_FATAL.

	[[nodiscard]]
	bool critical() const;
		/// Returns true if the log level is at least PRIO_CRITICAL.

	[[nodiscard]]
	bool error() const;
		/// Returns true if the log level is at least PRIO_ERROR.

	[[nodiscard]]
	bool warning() const;
		/// Returns true if the log level is at least PRIO_WARNING.

	[[nodiscard]]
	bool notice() const;
		/// Returns true if the log level is at least PRIO_NOTICE.

	[[nodiscard]]
	bool information() const;
		/// Returns true if the log level is at least PRIO_INFORMATION.

	[[nodiscard]]
	bool debug() const;
		/// Returns true if the log level is at least PRIO_DEBUG.

	[[nodiscard]]
	bool trace() const;
		/// Returns true if the log level is at least PRIO_TRACE.

	[[nodiscard]]
	static FastLogger& get(const std::string& name);
		/// Returns a reference to the FastLogger with the given name.
		/// If the logger does not exist, it is created based on its parent.

	[[nodiscard]]
	static FastLogger& unsafeGet(const std::string& name);
		/// Returns a reference to the FastLogger with the given name.
		/// Non-thread-safe version for use during initialization.

	[[nodiscard]]
	static FastLogger& create(const std::string& name, int level = Priority::PRIO_INFORMATION);
		/// Creates and returns a reference to a FastLogger with the
		/// given name and log level.

	[[nodiscard]]
	static FastLogger& create(const std::string& name, Channel::Ptr pChannel, int level = Priority::PRIO_INFORMATION);
		/// Creates and returns a reference to a FastLogger with the
		/// given name, Channel and log level. The Channel is accepted for
		/// API compatibility but ignored (Quill handles output internally).

	[[nodiscard]]
	static FastLogger& root();
		/// Returns a reference to the root logger.

	[[nodiscard]]
	static Ptr has(const std::string& name);
		/// Returns a pointer to the FastLogger if it exists, null otherwise.

	static void destroy(const std::string& name);
		/// Destroys the logger with the specified name.

	static void shutdown();
		/// Shuts down the logging framework, stopping the backend thread
		/// and flushing all pending messages.

	static void names(std::vector<std::string>& names);
		/// Fills the vector with names of all defined loggers.

	[[nodiscard]]
	static int parseLevel(const std::string& level);
		/// Parses a symbolic log level string and returns the numeric level.

	static void setLevel(const std::string& name, int level);
		/// Sets the log level on all loggers that are descendants of
		/// the logger with the given name.

	static void setChannel(const std::string& name, Channel::Ptr pChannel);
		/// Sets the Channel on all loggers that are descendants of
		/// the logger with the given name. For FastLogger this is a no-op
		/// as Quill handles output internally, but provided for API compatibility.

	static void setProperty(const std::string& loggerName, const std::string& propertyName, const std::string& value);
		/// Sets a property on all loggers that are descendants of
		/// the logger with the given name.

	[[nodiscard]]
	static std::string format(const std::string& fmt, const std::string& arg);
		/// Replaces all occurrences of $0 in fmt with the string given in arg and
		/// returns the result. To include a dollar sign in the result string,
		/// specify two dollar signs ($$) in the format string.

	[[nodiscard]]
	static std::string format(const std::string& fmt, const std::string& arg0, const std::string& arg1);
		/// Replaces all occurrences of $<n> in fmt with the string given in arg<n> and
		/// returns the result.

	[[nodiscard]]
	static std::string format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2);
		/// Replaces all occurrences of $<n> in fmt with the string given in arg<n> and
		/// returns the result.

	[[nodiscard]]
	static std::string format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2, const std::string& arg3);
		/// Replaces all occurrences of $<n> in fmt with the string given in arg<n> and
		/// returns the result.

	static void formatDump(std::string& message, const void* buffer, std::size_t length);
		/// Creates a hex-dump of the given buffer and appends it to the
		/// given message string.

	static void startBackend();
		/// Starts the Quill backend thread. Called automatically on first use.

	void flush();
		/// Blocks the calling thread until all log messages from this logger
		/// are flushed to the sinks. Use this when you need to ensure all
		/// pending messages are written before proceeding.
		///
		/// Note: This should not be called from a static destructor.

	static void setPattern(const std::string& pattern);
		/// Sets the log output pattern for the console sink.
		/// Uses Quill pattern syntax.

	static void addFileSink(const std::string& filename);
		/// Adds a file sink for log output.

	static void setBackendOption(const std::string& name, const std::string& value);
		/// Sets a Quill backend option before the backend is started.
		/// Must be called before any logging occurs.
		///
		/// Supported options:
		///   - threadName: Name of the backend thread (default: "QuillBackend")
		///   - sleepDuration: Backend sleep duration in microseconds (default: 100)
		///   - enableYieldWhenIdle: Enable yielding when idle (default: false)
		///   - enableCpuAffinity: Enable CPU affinity for backend thread (default: false)
		///   - cpuAffinity: Pin backend thread to specific CPU core (default: last core if enableCpuAffinity=true)
		///   - transitEventBufferInitialCapacity: Initial transit buffer size (default: 256)
		///   - transitEventsSoftLimit: Soft limit for transit events (default: 8192)
		///   - transitEventsHardLimit: Hard limit for transit events (default: 65536)
		///   - logTimestampOrderingGracePeriod: Grace period in microseconds (default: 5)
		///   - sinkMinFlushInterval: Min flush interval in milliseconds (default: 200)
		///   - waitForQueuesToEmptyBeforeExit: Wait on exit (default: true)

	static const std::string ROOT; /// The name of the root logger ("").

protected:
	using LoggerMap = std::map<std::string, Ptr>;

	FastLogger(const std::string& name, int level);
	~FastLogger() override;

	void logImpl(const std::string& text, Priority prio);
	void logImpl(const std::string& text, Priority prio, const char* file, LineNumber line);

	[[nodiscard]]
	static std::string formatImpl(const std::string& fmt, int argc, std::string argv[]);
	[[nodiscard]]
	static FastLogger& parent(const std::string& name);
	static void add(Ptr pLogger);
	[[nodiscard]]
	static Ptr find(const std::string& name);
	static void ensureBackendStarted();

private:
	using LoggerMapPtr = std::unique_ptr<LoggerMap>;

	FastLogger();
	FastLogger(const FastLogger&);
	FastLogger& operator = (const FastLogger&);

	std::string   _name;
	int           _level;
	void*         _pQuillLogger;  // quill::Logger* - opaque to avoid header dependency
	int           _sinkVersion;   // Incremented on each setChannel() to create unique Quill logger names

	static LoggerMapPtr _pLoggerMap;
	static Mutex        _mapMtx;
	static bool         _backendStarted;
	static Mutex        _backendMtx;
};


//
// inlines
//
inline const std::string& FastLogger::name() const
{
	return _name;
}


inline int FastLogger::getLevel() const
{
	return _level;
}


inline bool FastLogger::is(int level) const
{
	return _level >= level;
}


inline bool FastLogger::fatal() const
{
	return _level >= Priority::PRIO_FATAL;
}


inline bool FastLogger::critical() const
{
	return _level >= Priority::PRIO_CRITICAL;
}


inline bool FastLogger::error() const
{
	return _level >= Priority::PRIO_ERROR;
}


inline bool FastLogger::warning() const
{
	return _level >= Priority::PRIO_WARNING;
}


inline bool FastLogger::notice() const
{
	return _level >= Priority::PRIO_NOTICE;
}


inline bool FastLogger::information() const
{
	return _level >= Priority::PRIO_INFORMATION;
}


inline bool FastLogger::debug() const
{
	return _level >= Priority::PRIO_DEBUG;
}


inline bool FastLogger::trace() const
{
	return _level >= Priority::PRIO_TRACE;
}


} // namespace Poco


#endif // POCO_ENABLE_FASTLOGGER


#endif // Foundation_FastLogger_INCLUDED
