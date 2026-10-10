/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/LogLevel.h"
#include "quill/core/PatternFormatterOptions.h"
#include "quill/core/QuillError.h"
#include "quill/core/ThreadContextManager.h"

#include <atomic>
#include <memory>
#include <string>
#include <vector>

QUILL_BEGIN_NAMESPACE

/** Forward Declarations **/
class Sink;
class PatternFormatter;
class UserClockSource;

namespace detail
{
class BackendWorker;
class BacktraceStorage;
class LoggerManager;

/***/
class LoggerBase
{
public:
  /***/
  LoggerBase(std::string logger_name, std::vector<std::shared_ptr<Sink>> sinks,
             PatternFormatterOptions pattern_formatter_options, ClockSourceType clock_source,
             UserClockSource* user_clock)
    : _logger_name(static_cast<std::string&&>(logger_name)),
      _user_clock(user_clock),
      _clock_source(clock_source),
      _pattern_formatter_options(static_cast<PatternFormatterOptions&&>(pattern_formatter_options))
  {
#if defined(QUILL_ENABLE_ASSERTIONS) || !defined(NDEBUG)
    for (auto const& sink : sinks)
    {
      QUILL_ASSERT(sink, "sink pointer is nullptr in LoggerBase constructor");
    }
#endif

    this->_sinks = static_cast<std::vector<std::shared_ptr<Sink>>&&>(sinks);
  }

  /***/
  LoggerBase(LoggerBase const&) = delete;
  LoggerBase& operator=(LoggerBase const&) = delete;

  /***/
  virtual ~LoggerBase() = default;

  /**
   * Returns the name of the logger.
   * @return A constant reference to the logger's name.
   */
  QUILL_NODISCARD std::string const& get_logger_name() const noexcept { return _logger_name; }

  /**
   * Returns the user-defined clock source.
   * @return A pointer to the constant UserClockSource object.
   */
  QUILL_NODISCARD UserClockSource* get_user_clock_source() const noexcept { return _user_clock; }

  /**
   * Returns the type of clock source being used.
   * @return The ClockSourceType enum value representing the current clock source.
   */
  QUILL_NODISCARD ClockSourceType get_clock_source_type() const noexcept { return _clock_source; }

  /**
   * Returns the pattern formatter options.
   * @return A constant reference to the PatternFormatterOptions object.
   */
  QUILL_NODISCARD PatternFormatterOptions const& get_pattern_formatter_options() const noexcept
  {
    return _pattern_formatter_options;
  }

  /**
   * Returns a const reference to the sinks vector.
   * @warning The returned sinks should be treated as read-only. Do not modify the Sink objects as they are used by the backend thread
   * @return A constant reference to the std::vector<std::shared_ptr<Sink>> object
   */
  QUILL_NODISCARD std::vector<std::shared_ptr<Sink>> const& get_sinks() const noexcept
  {
    return _sinks;
  }

  /**
   * This function sets the logger's validity flag to false, indicating that the logger is no longer valid.
   */
  void mark_invalid() { _valid.store(false, std::memory_order_release); }

  /**
   * @brief Checks if the logger is valid.
   * @return True if the logger is valid, false otherwise.
   */
  QUILL_NODISCARD bool is_valid_logger() const noexcept
  {
    return _valid.load(std::memory_order_acquire);
  }

  /**
   * @return The log level of the logger
   */
  QUILL_NODISCARD LogLevel get_log_level() const noexcept
  {
    auto const* self = this;

#if defined(__GNUC__) && !defined(__clang__)
    // GCC warns about potential null pointer access when logger might be
    // uninitialized during LTO analysis
    asm volatile("" : "+r"(self) : : "memory");
#endif
    
    return self->_log_level.load(std::memory_order_relaxed);
  }

  /**
   * Set the log level of the logger
   * @param new_log_level The new log level
   */
  void set_log_level(LogLevel new_log_level)
  {
    if (QUILL_UNLIKELY(new_log_level == LogLevel::Backtrace))
    {
      QUILL_THROW(QuillError{"LogLevel::Backtrace is only used internally. Please don't use it."});
    }

    _log_level.store(new_log_level, std::memory_order_relaxed);
  }

  /**
   * Sets the flush interval for logging based on message count.
   *
   * When enabled, this will automatically call flush_log after the specified
   * number of messages have been logged
   *
   * This is particularly valuable when:
   * - Running the application in a debugger, pass flush_every_n_messages == 1
   * - Requiring synchronized logging for debugging, pass flush_every_n_messages == 1
   * - Dropping messages in a high-throughput simulation when using BoundedQueue, that you normally wouldn't drop in production environment
   *
   * @warning Enabling immediate flush can significantly impact application performance
   * and should only be used during debugging. It is disabled by default.
   *
   * @param flush_every_n_messages number of messages before auto-flush (0 to disable)
   */
  void set_immediate_flush(uint32_t flush_every_n_messages = 1)
  {
    _message_flush_threshold.store(flush_every_n_messages, std::memory_order_relaxed);
  }

  /**
   * Checks if the given log_statement_level can be logged by this logger
   * @return bool if a statement can be logged based on the current log level
   */
  template <LogLevel log_statement_level>
  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT bool should_log_statement() const noexcept
  {
    return log_statement_level >= get_log_level();
  }

  /**
   * Checks if the given log_statement_level can be logged by this logger
   * @param log_statement_level The log level of the log statement to be logged
   * @return bool if a statement can be logged based on the current log level
   */
  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT bool should_log_statement(LogLevel log_statement_level) const noexcept
  {
    return log_statement_level >= get_log_level();
  }

protected:
  friend class BackendWorker;
  friend class LoggerManager;

  static inline QUILL_THREAD_LOCAL ThreadContext* _thread_context = nullptr; /* Set and accessed by the frontend */

  // -- frontend access BEGIN --
  std::string _logger_name; /* Set by the frontend once, accessed by the frontend AND backend */
  UserClockSource* _user_clock{nullptr}; /* A non-owned pointer to a custom timestamp clock, valid only when provided. used by frontend only */
  std::atomic<uint32_t> _message_flush_threshold{0};   /* used by frontend only */
  std::atomic<uint32_t> _messages_since_last_flush{0}; /* used by frontend only */
  ClockSourceType _clock_source; /* Set by the frontend and accessed by the frontend AND backend */
  std::atomic<LogLevel> _log_level{LogLevel::Info}; /* used by frontend only */
  std::atomic<LogLevel> _backtrace_flush_level{LogLevel::None}; /** Updated by the frontend at any time, accessed by the backend */
  std::atomic<bool> _valid{true}; /* Updated by the frontend at any time, accessed by the backend */
  // -- frontend access END --

  PatternFormatterOptions _pattern_formatter_options; /* Set by the frontend once and accessed by the backend to initialise PatternFormatter */
  std::vector<std::shared_ptr<Sink>> _sinks; /* Set by the frontend once and accessed by the backend */
  std::shared_ptr<PatternFormatter> _pattern_formatter; /* The backend thread will set this once, we never access it on the frontend */
  std::shared_ptr<BacktraceStorage> _backtrace_storage; /* The backend thread will construct this, we never access it on the frontend */
};
} // namespace detail

QUILL_END_NAMESPACE