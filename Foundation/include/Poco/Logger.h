//
// Logger.h
//
// Library: Foundation
// Package: Logging
// Module:  Logger
//
// Definition of the Logger class.
//
// Copyright (c) 2004-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Logger_INCLUDED
#define Foundation_Logger_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Channel.h"
#include "Poco/Message.h"
#include "Poco/Format.h"
#include "Poco/AutoPtr.h"
#include <map>
#include <vector>
#include <cstddef>
#include <memory>


namespace Poco {


class Exception;


class Foundation_API Logger: public Channel
	/// Logger is a special Channel that acts as the main
	/// entry point into the logging framework.
	///
	/// An application uses instances of the Logger class to generate its log messages
	/// and send them on their way to their final destination. Logger instances
	/// are organized in a hierarchical, tree-like manner and are maintained by
	/// the framework. Every Logger object has exactly one direct ancestor, with
	/// the exception of the root logger. A newly created logger inherits its properties -
	/// channel and level - from its direct ancestor. Every logger is connected
	/// to a channel, to which it passes on its messages. Furthermore, every logger
	/// has a log level, which is used for filtering messages based on their priority.
	/// Only messages with a priority equal to or higher than the specified level
	/// are passed on. For example, if the level of a logger is set to three (PRIO_ERROR),
	/// only messages with priority PRIO_ERROR, PRIO_CRITICAL and PRIO_FATAL will
	/// propagate. If the level is set to zero, the logger is effectively disabled.
	///
	/// The name of a logger determines the logger's place within the logger hierarchy.
	/// The name of the root logger is always "", the empty string. For all other
	/// loggers, the name is made up of one or more components, separated by a period.
	/// For example, the loggers with the name HTTPServer.RequestHandler and
	/// HTTPServer.Listener are descendants of the logger HTTPServer, which itself is a
	/// descendant of the root logger. There is no limit as to how deep the logger hierarchy
	/// can become. Once a logger has been created and it has inherited the channel and level
	/// from its ancestor, it loses the connection to it. So, changes to the level or
	/// channel of a logger do not affect its descendants. This greatly simplifies the
	/// implementation of the framework and is no real restriction, because almost always
	/// levels and channels are set up at application startup and never changed afterwards.
	/// Nevertheless, there are methods to simultaneously change the level and channel of
	/// all loggers in a certain hierarchy.
	///
	/// There are also convenience macros available that wrap the actual
	/// logging statement into a check whether the Logger's log level
	/// is sufficient to actually log the message. This allows to increase
	/// the application performance if many complex log statements
	/// are used. The macros also add the source file path and line
	/// number into the log message so that it is available to formatters.
	/// Variants of these macros that allow message formatting with Poco::format()
	/// are also available.
	///
	/// Examples:
	///     poco_warning(logger, "This is a warning");
	///     poco_information_f(logger, "An informational message with args: %d, %d", 1, 2);
{
public:
	using Ptr = AutoPtr<Logger>;

	const std::string& name() const;
		/// Returns the name of the logger, which is set as the
		/// message source on all messages created by the logger.

	void setChannel(Channel::Ptr pChannel);
		/// Attaches the given Channel to the Logger.

	Channel::Ptr getChannel() const;
		/// Returns the Channel attached to the logger.

	void setLevel(int level);
		/// Sets the Logger's log level.
		///
		/// See Message::Priority for valid log levels.
		/// Setting the log level to zero turns off
		/// logging for that Logger.

	int getLevel() const;
		/// Returns the Logger's log level.

	void setLevel(const std::string& level);
		/// Sets the Logger's log level using a symbolic value.
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

	void setProperty(const std::string& name, const std::string& value);
		/// Sets or changes a configuration property.
		///
		/// Only the "channel" and "level" properties are supported, which allow
		/// setting the target channel and log level, respectively, via the LoggingRegistry.
		/// The "channel" and "level" properties are set-only.

	void log(const Message& msg);
		/// Logs the given message if its priority is
		/// greater than or equal to the Logger's log level.

	void log(const Exception& exc);
		/// Logs the given exception with priority PRIO_ERROR.

	void log(const Exception& exc, const char* file, LineNumber line);
		/// Logs the given exception with priority PRIO_ERROR.
		///
		/// File must be a static string, such as the value of
		/// the __FILE__ macro. The string is not copied
		/// internally for performance reasons.

	void fatal(const std::string& msg);
		/// If the Logger's log level is at least PRIO_FATAL,
		/// creates a Message with priority PRIO_FATAL
		/// and the given message text and sends it
		/// to the attached channel.
	
	void fatal(const std::string& msg, const char* file, LineNumber line);
		/// If the Logger's log level is at least PRIO_FATAL,
		/// creates a Message with priority PRIO_FATAL
		/// and the given message text and sends it
		/// to the attached channel.
		///
		/// File must be a static string, such as the value of
		/// the __FILE__ macro. The string is not copied
		/// internally for performance reasons.

	template <typename T, typename... Args>
	void fatal(const std::string& fmt, T arg1, Args&&... args)
	{
		if (fatal())
			logAlways(Poco::format(fmt, arg1, std::forward<Args>(args)...), Message::PRIO_FATAL);
	}

	void critical(const std::string& msg);
		/// If the Logger's log level is at least PRIO_CRITICAL,
		/// creates a Message with priority PRIO_CRITICAL
		/// and the given message text and sends it
		/// to the attached channel.

	void critical(const std::string& msg, const char* file, LineNumber line);
		/// If the Logger's log level is at least PRIO_CRITICAL,
		/// creates a Message with priority PRIO_CRITICAL
		/// and the given message text and sends it
		/// to the attached channel.
		///
		/// File must be a static string, such as the value of
		/// the __FILE__ macro. The string is not copied
		/// internally for performance reasons.

	template <typename T, typename... Args>
	void critical(const std::string& fmt, T arg1, Args&&... args)
	{
		if (critical())
			logAlways(Poco::format(fmt, arg1, std::forward<Args>(args)...), Message::PRIO_CRITICAL);
	}

	void error(const std::string& msg);
		/// If the Logger's log level is at least PRIO_ERROR,
		/// creates a Message with priority PRIO_ERROR
		/// and the given message text and sends it
		/// to the attached channel.

	void error(const std::string& msg, const char* file, LineNumber line);
		/// If the Logger's log level is at least PRIO_ERROR,
		/// creates a Message with priority PRIO_ERROR
		/// and the given message text and sends it
		/// to the attached channel.
		///
		/// File must be a static string, such as the value of
		/// the __FILE__ macro. The string is not copied
		/// internally for performance reasons.

	template <typename T, typename... Args>
	void error(const std::string& fmt, T arg1, Args&&... args)
	{
		if (error())
			logAlways(Poco::format(fmt, arg1, std::forward<Args>(args)...), Message::PRIO_ERROR);
	}

	void warning(const std::string& msg);
		/// If the Logger's log level is at least PRIO_WARNING,
		/// creates a Message with priority PRIO_WARNING
		/// and the given message text and sends it
		/// to the attached channel.

	void warning(const std::string& msg, const char* file, LineNumber line);
		/// If the Logger's log level is at least PRIO_WARNING,
		/// creates a Message with priority PRIO_WARNING
		/// and the given message text and sends it
		/// to the attached channel.
		///
		/// File must be a static string, such as the value of
		/// the __FILE__ macro. The string is not copied
		/// internally for performance reasons.

	template <typename T, typename... Args>
	void warning(const std::string& fmt, T arg1, Args&&... args)
	{
		if (warning())
			logAlways(Poco::format(fmt, arg1, std::forward<Args>(args)...), Message::PRIO_WARNING);
	}

	void notice(const std::string& msg);
		/// If the Logger's log level is at least PRIO_NOTICE,
		/// creates a Message with priority PRIO_NOTICE
		/// and the given message text and sends it
		/// to the attached channel.

	void notice(const std::string& msg, const char* file, LineNumber line);
		/// If the Logger's log level is at least PRIO_NOTICE,
		/// creates a Message with priority PRIO_NOTICE
		/// and the given message text and sends it
		/// to the attached channel.
		///
		/// File must be a static string, such as the value of
		/// the __FILE__ macro. The string is not copied
		/// internally for performance reasons.

	template <typename T, typename... Args>
	void notice(const std::string& fmt, T arg1, Args&&... args)
	{
		if (notice())
			logAlways(Poco::format(fmt, arg1, std::forward<Args>(args)...), Message::PRIO_NOTICE);
	}

	void information(const std::string& msg);
		/// If the Logger's log level is at least PRIO_INFORMATION,
		/// creates a Message with priority PRIO_INFORMATION
		/// and the given message text and sends it
		/// to the attached channel.

	void information(const std::string& msg, const char* file, LineNumber line);
		/// If the Logger's log level is at least PRIO_INFORMATION,
		/// creates a Message with priority PRIO_INFORMATION
		/// and the given message text and sends it
		/// to the attached channel.
		///
		/// File must be a static string, such as the value of
		/// the __FILE__ macro. The string is not copied
		/// internally for performance reasons.

	template <typename T, typename... Args>
	void information(const std::string& fmt, T arg1, Args&&... args)
	{
		if (information())
			logAlways(Poco::format(fmt, arg1, std::forward<Args>(args)...), Message::PRIO_INFORMATION);
	}

	void debug(const std::string& msg);
		/// If the Logger's log level is at least PRIO_DEBUG,
		/// creates a Message with priority PRIO_DEBUG
		/// and the given message text and sends it
		/// to the attached channel.

	void debug(const std::string& msg, const char* file, LineNumber line);
		/// If the Logger's log level is at least PRIO_DEBUG,
		/// creates a Message with priority PRIO_DEBUG
		/// and the given message text and sends it
		/// to the attached channel.
		///
		/// File must be a static string, such as the value of
		/// the __FILE__ macro. The string is not copied
		/// internally for performance reasons.

	template <typename T, typename... Args>
	void debug(const std::string& fmt, T arg1, Args&&... args)
	{
		if (debug())
			logAlways(Poco::format(fmt, arg1, std::forward<Args>(args)...), Message::PRIO_DEBUG);
	}

	void trace(const std::string& msg);
		/// If the Logger's log level is at least PRIO_TRACE,
		/// creates a Message with priority PRIO_TRACE
		/// and the given message text and sends it
		/// to the attached channel.

	void trace(const std::string& msg, const char* file, LineNumber line);
		/// If the Logger's log level is at least PRIO_TRACE,
		/// creates a Message with priority PRIO_TRACE
		/// and the given message text and sends it
		/// to the attached channel.
		///
		/// File must be a static string, such as the value of
		/// the __FILE__ macro. The string is not copied
		/// internally for performance reasons.

	template <typename T, typename... Args>
	void trace(const std::string& fmt, T arg1, Args&&... args)
	{
		if (trace())
			logAlways(Poco::format(fmt, arg1, std::forward<Args>(args)...), Message::PRIO_TRACE);
	}

	void dump(const std::string& msg, const void* buffer, std::size_t length, Message::Priority prio = Message::PRIO_DEBUG);
		/// Logs the given message, followed by the data in buffer.
		///
		/// The data in buffer is written in canonical hex+ASCII form:
		/// Offset (4 bytes) in hexadecimal, followed by sixteen
		/// space-separated, two column, hexadecimal bytes,
		/// followed by the same sixteen bytes as ASCII characters.
		/// For bytes outside the range 32 .. 127, a dot is printed.

	bool is(int level) const;
		/// Returns true if at least the given log level is set.

	bool fatal() const;
		/// Returns true if the log level is at least PRIO_FATAL.

	bool critical() const;
		/// Returns true if the log level is at least PRIO_CRITICAL.

	bool error() const;
		/// Returns true if the log level is at least PRIO_ERROR.

	bool warning() const;
		/// Returns true if the log level is at least PRIO_WARNING.

	bool notice() const;
		/// Returns true if the log level is at least PRIO_NOTICE.

	bool information() const;
		/// Returns true if the log level is at least PRIO_INFORMATION.

	bool debug() const;
		/// Returns true if the log level is at least PRIO_DEBUG.

	bool trace() const;
		/// Returns true if the log level is at least PRIO_TRACE.

	static std::string format(const std::string& fmt, const std::string& arg);
		/// Replaces all occurrences of $0 in fmt with the string given in arg and
		/// returns the result. To include a dollar sign in the result string,
		/// specify two dollar signs ($$) in the format string.

	static std::string format(const std::string& fmt, const std::string& arg0, const std::string& arg1);
		/// Replaces all occurrences of $<n> in fmt with the string given in arg<n> and
		/// returns the result. To include a dollar sign in the result string,
		/// specify two dollar signs ($$) in the format string.

	static std::string format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2);
		/// Replaces all occurrences of $<n> in fmt with the string given in arg<n> and
		/// returns the result. To include a dollar sign in the result string,
		/// specify two dollar signs ($$) in the format string.

	static std::string format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2, const std::string& arg3);
		/// Replaces all occurrences of $<n> in fmt with the string given in arg<n> and
		/// returns the result. To include a dollar sign in the result string,
		/// specify two dollar signs ($$) in the format string.

	static void formatDump(std::string& message, const void* buffer, std::size_t length);
		/// Creates a hex-dump of the given buffer and appends it to the
		/// given message string.

	static void setLevel(const std::string& name, int level);
		/// Sets the given log level on all loggers that are
		/// descendants of the Logger with the given name.

	static void setChannel(const std::string& name, Channel::Ptr pChannel);
		/// Attaches the given Channel to all loggers that are
		/// descendants of the Logger with the given name.

	static void setProperty(const std::string& loggerName, const std::string& propertyName, const std::string& value);
		/// Sets or changes a configuration property for all loggers
		/// that are descendants of the Logger with the given name.

	static Logger& get(const std::string& name);
		/// Returns a reference to the Logger with the given name.
		/// If the Logger does not yet exist, it is created, based
		/// on its parent logger.

	static Logger& unsafeGet(const std::string& name);
		/// Returns a reference to the Logger with the given name.
		/// If the Logger does not yet exist, it is created, based
		/// on its parent logger.
		///
		/// WARNING: This method is not thread safe. You should
		/// probably use get() instead.
		/// The only time this method should be used is during
		/// program initialization, when only one thread is running.

	static Logger& create(const std::string& name, Channel::Ptr pChannel, int level = Message::PRIO_INFORMATION);
		/// Creates and returns a reference to a Logger with the
		/// given name. The Logger's Channel and log level as set as
		/// specified.

	static Logger& root();
		/// Returns a reference to the root logger, which is the ultimate
		/// ancestor of all Loggers.

	static Ptr has(const std::string& name);
		/// Returns a pointer to the Logger with the given name if it
		/// exists, or a null pointer otherwise.

	static void destroy(const std::string& name);
		/// Destroys the logger with the specified name. Does nothing
		/// if the logger is not found.
		///
		/// After a logger has been destroyed, all references to it
		/// become invalid.

	static void shutdown();
		/// Shuts down the logging framework and releases all
		/// Loggers.

	static void names(std::vector<std::string>& names);
		/// Fills the given vector with the names
		/// of all currently defined loggers.

	static int parseLevel(const std::string& level);
		/// Parses a symbolic log level from a string and
		/// returns the resulting numeric level.
		///
		/// Valid symbolic levels are:
		///   - none (turns off logging)
		///   - fatal
		///   - critical
		///   - error
		///   - warning
		///   - notice
		///   - information
		///   - debug
		///   - trace
		///
		/// The level is not case sensitive.

	static const std::string ROOT; /// The name of the root logger ("").

protected:
	typedef std::map<std::string, Ptr> LoggerMap;

	Logger(const std::string& name, Channel::Ptr pChannel, int level);
	~Logger();

	void log(const std::string& text, Message::Priority prio);
	void logNPC(const std::string& text, Message::Priority prio);
	void log(const std::string& text, Message::Priority prio, const char* file, LineNumber line);

	static std::string format(const std::string& fmt, int argc, std::string argv[]);
	static Logger& parent(const std::string& name);
	static void add(Ptr pLogger);
	static Ptr find(const std::string& name);

private:
	typedef std::unique_ptr<LoggerMap> LoggerMapPtr;

	Logger();
	Logger(const Logger&);
	Logger& operator = (const Logger&);

	void logAlways(const std::string& text, Message::Priority prio);

	std::string _name;
	Channel::Ptr _pChannel;
	int         _level;

	// definitions in Foundation.cpp
	static LoggerMapPtr _pLoggerMap;
	static Mutex      _mapMtx;
};


//
// convenience macros
//

// The GNU compiler emits a warning if nested "if" statements are followed by
// an "else" statement and braces are not used to explicitly disambiguate the
// "else" binding.	This leads to problems with code like:
//
//	 if (gate)
//		 poco_log(loglevel, msg);
//
#ifdef __INTEL_COMPILER
#define POCO_AMBIGUOUS_ELSE_BLOCKER_BEG
#define POCO_AMBIGUOUS_ELSE_BLOCKER_END
#else
#define POCO_AMBIGUOUS_ELSE_BLOCKER_BEG do {

#define POCO_AMBIGUOUS_ELSE_BLOCKER_END \
	} while(0)
#endif

#define poco_fatal(logger, msg) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).fatal()) (logger).fatal(msg, __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_fatal_f1(logger, fmt, arg1) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).fatal()) (logger).fatal(Poco::format((fmt), arg1), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_fatal_f2(logger, fmt, arg1, arg2) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).fatal()) (logger).fatal(Poco::format((fmt), (arg1), (arg2)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_fatal_f3(logger, fmt, arg1, arg2, arg3) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).fatal()) (logger).fatal(Poco::format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_fatal_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).fatal()) (logger).fatal(Poco::format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_fatal_f(logger, fmt, ...) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).fatal()) (logger).fatal(Poco::format((fmt), __VA_ARGS__), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_critical(logger, msg) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).critical()) (logger).critical(msg, __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_critical_f1(logger, fmt, arg1) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).critical()) (logger).critical(Poco::format((fmt), (arg1)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_critical_f2(logger, fmt, arg1, arg2) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).critical()) (logger).critical(Poco::format((fmt), (arg1), (arg2)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_critical_f3(logger, fmt, arg1, arg2, arg3) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).critical()) (logger).critical(Poco::format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_critical_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).critical()) (logger).critical(Poco::format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_critical_f(logger, fmt, ...) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).critical()) (logger).critical(Poco::format((fmt), __VA_ARGS__), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_error(logger, msg) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).error()) (logger).error(msg, __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_error_f1(logger, fmt, arg1) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).error()) (logger).error(Poco::format((fmt), (arg1)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_error_f2(logger, fmt, arg1, arg2) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).error()) (logger).error(Poco::format((fmt), (arg1), (arg2)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_error_f3(logger, fmt, arg1, arg2, arg3) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).error()) (logger).error(Poco::format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_error_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).error()) (logger).error(Poco::format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_error_f(logger, fmt, ...) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).error()) (logger).error(Poco::format((fmt), __VA_ARGS__), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_warning(logger, msg) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).warning()) (logger).warning(msg, __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_warning_f1(logger, fmt, arg1) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).warning()) (logger).warning(Poco::format((fmt), (arg1)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_warning_f2(logger, fmt, arg1, arg2) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).warning()) (logger).warning(Poco::format((fmt), (arg1), (arg2)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_warning_f3(logger, fmt, arg1, arg2, arg3) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).warning()) (logger).warning(Poco::format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_warning_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).warning()) (logger).warning(Poco::format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_warning_f(logger, fmt, ...) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).warning()) (logger).warning(Poco::format((fmt), __VA_ARGS__), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_notice(logger, msg) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).notice()) (logger).notice(msg, __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_notice_f1(logger, fmt, arg1) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).notice()) (logger).notice(Poco::format((fmt), (arg1)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_notice_f2(logger, fmt, arg1, arg2) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).notice()) (logger).notice(Poco::format((fmt), (arg1), (arg2)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_notice_f3(logger, fmt, arg1, arg2, arg3) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).notice()) (logger).notice(Poco::format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_notice_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).notice()) (logger).notice(Poco::format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_notice_f(logger, fmt, ...) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).notice()) (logger).notice(Poco::format((fmt), __VA_ARGS__), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_information(logger, msg) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).information()) (logger).information(msg, __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_information_f1(logger, fmt, arg1) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).information()) (logger).information(Poco::format((fmt), (arg1)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_information_f2(logger, fmt, arg1, arg2) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).information()) (logger).information(Poco::format((fmt), (arg1), (arg2)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_information_f3(logger, fmt, arg1, arg2, arg3) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).information()) (logger).information(Poco::format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_information_f4(logger, fmt, arg1, arg2, arg3, arg4) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).information()) (logger).information(Poco::format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_information_f(logger, fmt, ...) \
	POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
	if ((logger).information()) (logger).information(Poco::format((fmt), __VA_ARGS__), __FILE__, __LINE__); \
	POCO_AMBIGUOUS_ELSE_BLOCKER_END

#if defined(_DEBUG) || defined(POCO_LOG_DEBUG)
#define poco_debug(logger, msg) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).debug()) (logger).debug(msg, __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_debug_f1(logger, fmt, arg1) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).debug()) (logger).debug(Poco::format((fmt), (arg1)), __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_debug_f2(logger, fmt, arg1, arg2) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).debug()) (logger).debug(Poco::format((fmt), (arg1), (arg2)), __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_debug_f3(logger, fmt, arg1, arg2, arg3) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).debug()) (logger).debug(Poco::format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_debug_f4(logger, fmt, arg1, arg2, arg3, arg4) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).debug()) (logger).debug(Poco::format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_debug_f(logger, fmt, ...) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).debug()) (logger).debug(Poco::format((fmt), __VA_ARGS__), __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_trace(logger, msg) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).trace()) (logger).trace(msg, __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_trace_f1(logger, fmt, arg1) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).trace()) (logger).trace(Poco::format((fmt), (arg1)), __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_trace_f2(logger, fmt, arg1, arg2) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).trace()) (logger).trace(Poco::format((fmt), (arg1), (arg2)), __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_trace_f3(logger, fmt, arg1, arg2, arg3) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).trace()) (logger).trace(Poco::format((fmt), (arg1), (arg2), (arg3)), __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_trace_f4(logger, fmt, arg1, arg2, arg3, arg4) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).trace()) (logger).trace(Poco::format((fmt), (arg1), (arg2), (arg3), (arg4)), __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END

#define poco_trace_f(logger, fmt, ...) \
		POCO_AMBIGUOUS_ELSE_BLOCKER_BEG \
		if ((logger).trace()) (logger).trace(Poco::format((fmt), __VA_ARGS__), __FILE__, __LINE__); \
		POCO_AMBIGUOUS_ELSE_BLOCKER_END
#else
	#define poco_debug(logger, msg)
	#define poco_debug_f1(logger, fmt, arg1)
	#define poco_debug_f2(logger, fmt, arg1, arg2)
	#define poco_debug_f3(logger, fmt, arg1, arg2, arg3)
	#define poco_debug_f4(logger, fmt, arg1, arg2, arg3, arg4)
	#define poco_debug_f(logger, fmt, ...)
	#define poco_trace(logger, msg)
	#define poco_trace_f1(logger, fmt, arg1)
	#define poco_trace_f2(logger, fmt, arg1, arg2)
	#define poco_trace_f3(logger, fmt, arg1, arg2, arg3)
	#define poco_trace_f4(logger, fmt, arg1, arg2, arg3, arg4)
	#define poco_trace_f(logger, fmt, ...)
#endif


//
// inlines
//
inline const std::string& Logger::name() const
{
	return _name;
}


inline int Logger::getLevel() const
{
	return _level;
}


inline void Logger::log(const std::string& text, Message::Priority prio)
{
	if (_level >= prio && _pChannel)
	{
		_pChannel->log(Message(_name, text, prio));
	}
}


inline void Logger::logNPC(const std::string& text, Message::Priority prio)
{
	if (_pChannel)
	{
		_pChannel->log(Message(_name, text, prio));
	}
}


inline void Logger::log(const std::string& text, Message::Priority prio, const char* file, LineNumber line)
{
	if (_level >= prio && _pChannel)
	{
		_pChannel->log(Message(_name, text, prio, file, line));
	}
}


inline void Logger::logAlways(const std::string& text, Message::Priority prio)
{
	if (_pChannel)
	{
		_pChannel->log(Message(_name, text, prio));
	}
}


inline void Logger::fatal(const std::string& msg)
{
	log(msg, Message::PRIO_FATAL);
}


inline void Logger::fatal(const std::string& msg, const char* file, LineNumber line)
{
	log(msg, Message::PRIO_FATAL, file, line);
}



inline void Logger::critical(const std::string& msg)
{
	log(msg, Message::PRIO_CRITICAL);
}


inline void Logger::critical(const std::string& msg, const char* file, LineNumber line)
{
	log(msg, Message::PRIO_CRITICAL, file, line);
}


inline void Logger::error(const std::string& msg)
{
	log(msg, Message::PRIO_ERROR);
}


inline void Logger::error(const std::string& msg, const char* file, LineNumber line)
{
	log(msg, Message::PRIO_ERROR, file, line);
}


inline void Logger::warning(const std::string& msg)
{
	log(msg, Message::PRIO_WARNING);
}


inline void Logger::warning(const std::string& msg, const char* file, LineNumber line)
{
	log(msg, Message::PRIO_WARNING, file, line);
}


inline void Logger::notice(const std::string& msg)
{
	log(msg, Message::PRIO_NOTICE);
}


inline void Logger::notice(const std::string& msg, const char* file, LineNumber line)
{
	log(msg, Message::PRIO_NOTICE, file, line);
}


inline void Logger::information(const std::string& msg)
{
	log(msg, Message::PRIO_INFORMATION);
}


inline void Logger::information(const std::string& msg, const char* file, LineNumber line)
{
	log(msg, Message::PRIO_INFORMATION, file, line);
}


inline void Logger::debug(const std::string& msg)
{
	log(msg, Message::PRIO_DEBUG);
}


inline void Logger::debug(const std::string& msg, const char* file, LineNumber line)
{
	log(msg, Message::PRIO_DEBUG, file, line);
}


inline void Logger::trace(const std::string& msg)
{
	log(msg, Message::PRIO_TRACE);
}


inline void Logger::trace(const std::string& msg, const char* file, LineNumber line)
{
	log(msg, Message::PRIO_TRACE, file, line);
}


inline bool Logger::is(int level) const
{
	return _level >= level;
}


inline bool Logger::fatal() const
{
	return _level >= Message::PRIO_FATAL;
}


inline bool Logger::critical() const
{
	return _level >= Message::PRIO_CRITICAL;
}


inline bool Logger::error() const
{
	return _level >= Message::PRIO_ERROR;
}


inline bool Logger::warning() const
{
	return _level >= Message::PRIO_WARNING;
}


inline bool Logger::notice() const
{
	return _level >= Message::PRIO_NOTICE;
}


inline bool Logger::information() const
{
	return _level >= Message::PRIO_INFORMATION;
}


inline bool Logger::debug() const
{
	return _level >= Message::PRIO_DEBUG;
}


inline bool Logger::trace() const
{
	return _level >= Message::PRIO_TRACE;
}


} // namespace Poco


#endif // Foundation_Logger_INCLUDED
