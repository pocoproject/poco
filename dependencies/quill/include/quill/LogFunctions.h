/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/LogLevel.h"
#include "quill/core/MacroMetadata.h"
#include "quill/core/SourceLocation.h"

#include <string>
#include <type_traits>
#include <utility>

QUILL_BEGIN_NAMESPACE

/**
 * # Macro-Free Logging Interface
 *
 * This file provides an alternative way to log messages without using macros, offering
 * cleaner code in some scenarios at the cost of some performance.
 *
 * ## Requirements
 *
 * The macro-free mode is implemented using  __builtin_FILE(), __builtin_FUNCTION(), __builtin_LINE() which may vary by compiler:
 *
 * ## Performance Trade-offs
 *
 * While convenient, the macro-free approach has several performance implications compared to macros:
 *
 * 1. Higher Latency: Format metadata isn't available at compile time, requiring additional
 *    runtime copying of metadata to the backend thread
 *
 * 2. Always-Evaluated Arguments: Unlike macros which skip evaluation for disabled log levels,
 *    arguments to these functions are always evaluated
 *
 * 3. No Compile-Time Removal: Cannot be completely compiled out with
 *    QUILL_COMPILE_ACTIVE_LOG_LEVEL_<LEVEL> as macros can
 *
 * 4. Backend Thread Impact: Slightly reduced throughput in the backend due to
 *    runtime metadata storage and processing
 *
 * For performance-critical logging paths, use the macro-based logging interface.
 */

/**
 * Tag structure for log messages that supports multiple tags
 */
struct Tags
{
  Tags() = default;

  /**
   * Single tag constructor
   * @param tag Tag to add to the log message
   */
  explicit Tags(char const* tag)
  {
    if (QUILL_UNLIKELY(!tag))
    {
      return;
    }

    _value += tag;
    _value += ' ';
  }

  /**
   * Constructor for multiple tags
   * @param first_tag Tag to add to the log message
   * @param second_tag Tag to add to the log message
   * @param rest_tags Tag to add to the log message
   */
  template <typename... RestTags>
  Tags(char const* first_tag, char const* second_tag, RestTags... rest_tags)
  {
    static_assert(std::conjunction_v<std::is_same<char const*, RestTags>...>,
                  "All tag parameters must be of type 'char const*'");

    append_tag(first_tag);
    append_tag(second_tag);
    (append_tag(rest_tags), ...);
    _value += ' ';
  }

  // Get the combined tags string
  QUILL_NODISCARD char const* value() const noexcept { return _value.c_str(); }

private:
  std::string _value{'#'};

  void append_tag(char const* tag)
  {
    if (QUILL_UNLIKELY(!tag))
    {
      return;
    }

    if (_value.size() > 1)
    {
      // when not empty, 1 is for initial '#'
      _value += " #";
    }

    _value += tag;
  }
};

template <typename TLogger, typename... Args>
struct tracel3
{
  tracel3(TLogger* logger, char const* fmt, Args&&... args, SourceLocation location = SourceLocation::current())
  {
    log(logger, "", LogLevel::TraceL3, fmt, location, std::forward<Args>(args)...);
  }

  tracel3(TLogger* logger, Tags const& tags, char const* fmt, Args&&... args,
          SourceLocation location = SourceLocation::current())
  {
    log(logger, tags.value(), LogLevel::TraceL3, fmt, location, std::forward<Args>(args)...);
  }
};

template <typename TLogger, typename... Args>
tracel3(TLogger*, char const*, Args&&...) -> tracel3<TLogger, Args...>;

template <typename TLogger, typename... Args>
tracel3(TLogger*, Tags const& tags, char const*, Args&&...) -> tracel3<TLogger, Args...>;

template <typename TLogger, typename... Args>
struct tracel2
{
  tracel2(TLogger* logger, char const* fmt, Args&&... args, SourceLocation location = SourceLocation::current())
  {
    log(logger, "", LogLevel::TraceL2, fmt, location, std::forward<Args>(args)...);
  }

  tracel2(TLogger* logger, Tags const& tags, char const* fmt, Args&&... args,
          SourceLocation location = SourceLocation::current())
  {
    log(logger, tags.value(), LogLevel::TraceL2, fmt, location, std::forward<Args>(args)...);
  }
};

template <typename TLogger, typename... Args>
tracel2(TLogger*, char const*, Args&&...) -> tracel2<TLogger, Args...>;

template <typename TLogger, typename... Args>
tracel2(TLogger*, Tags const& tags, char const*, Args&&...) -> tracel2<TLogger, Args...>;

template <typename TLogger, typename... Args>
struct tracel1
{
  tracel1(TLogger* logger, char const* fmt, Args&&... args, SourceLocation location = SourceLocation::current())
  {
    log(logger, "", LogLevel::TraceL1, fmt, location, std::forward<Args>(args)...);
  }

  tracel1(TLogger* logger, Tags const& tags, char const* fmt, Args&&... args,
          SourceLocation location = SourceLocation::current())
  {
    log(logger, tags.value(), LogLevel::TraceL1, fmt, location, std::forward<Args>(args)...);
  }
};

template <typename TLogger, typename... Args>
tracel1(TLogger*, char const*, Args&&...) -> tracel1<TLogger, Args...>;

template <typename TLogger, typename... Args>
tracel1(TLogger*, Tags const& tags, char const*, Args&&...) -> tracel1<TLogger, Args...>;

template <typename TLogger, typename... Args>
struct debug
{
  debug(TLogger* logger, char const* fmt, Args&&... args, SourceLocation location = SourceLocation::current())
  {
    log(logger, "", LogLevel::Debug, fmt, location, std::forward<Args>(args)...);
  }

  debug(TLogger* logger, Tags const& tags, char const* fmt, Args&&... args,
        SourceLocation location = SourceLocation::current())
  {
    log(logger, tags.value(), LogLevel::Debug, fmt, location, std::forward<Args>(args)...);
  }
};

template <typename TLogger, typename... Args>
debug(TLogger*, char const*, Args&&...) -> debug<TLogger, Args...>;

template <typename TLogger, typename... Args>
debug(TLogger*, Tags const& tags, char const*, Args&&...) -> debug<TLogger, Args...>;

template <typename TLogger, typename... Args>
struct info
{
  info(TLogger* logger, char const* fmt, Args&&... args, SourceLocation location = SourceLocation::current())
  {
    log(logger, "", LogLevel::Info, fmt, location, std::forward<Args>(args)...);
  }

  info(TLogger* logger, Tags const& tags, char const* fmt, Args&&... args,
       SourceLocation location = SourceLocation::current())
  {
    log(logger, tags.value(), LogLevel::Info, fmt, location, std::forward<Args>(args)...);
  }
};

template <typename TLogger, typename... Args>
info(TLogger*, char const*, Args&&...) -> info<TLogger, Args...>;

template <typename TLogger, typename... Args>
info(TLogger*, Tags const& tags, char const*, Args&&...) -> info<TLogger, Args...>;

template <typename TLogger, typename... Args>
struct notice
{
  notice(TLogger* logger, char const* fmt, Args&&... args, SourceLocation location = SourceLocation::current())
  {
    log(logger, "", LogLevel::Notice, fmt, location, std::forward<Args>(args)...);
  }

  notice(TLogger* logger, Tags const& tags, char const* fmt, Args&&... args,
         SourceLocation location = SourceLocation::current())
  {
    log(logger, tags.value(), LogLevel::Notice, fmt, location, std::forward<Args>(args)...);
  }
};

template <typename TLogger, typename... Args>
notice(TLogger*, char const*, Args&&...) -> notice<TLogger, Args...>;

template <typename TLogger, typename... Args>
notice(TLogger*, Tags const& tags, char const*, Args&&...) -> notice<TLogger, Args...>;

template <typename TLogger, typename... Args>
struct warning
{
  warning(TLogger* logger, char const* fmt, Args&&... args, SourceLocation location = SourceLocation::current())
  {
    log(logger, "", LogLevel::Warning, fmt, location, std::forward<Args>(args)...);
  }

  warning(TLogger* logger, Tags const& tags, char const* fmt, Args&&... args,
          SourceLocation location = SourceLocation::current())
  {
    log(logger, tags.value(), LogLevel::Warning, fmt, location, std::forward<Args>(args)...);
  }
};

template <typename TLogger, typename... Args>
warning(TLogger*, char const*, Args&&...) -> warning<TLogger, Args...>;

template <typename TLogger, typename... Args>
warning(TLogger*, Tags const& tags, char const*, Args&&...) -> warning<TLogger, Args...>;

template <typename TLogger, typename... Args>
struct error
{
  error(TLogger* logger, char const* fmt, Args&&... args, SourceLocation location = SourceLocation::current())
  {
    log(logger, "", LogLevel::Error, fmt, location, std::forward<Args>(args)...);
  }

  error(TLogger* logger, Tags const& tags, char const* fmt, Args&&... args,
        SourceLocation location = SourceLocation::current())
  {
    log(logger, tags.value(), LogLevel::Error, fmt, location, std::forward<Args>(args)...);
  }
};

template <typename TLogger, typename... Args>
error(TLogger*, char const*, Args&&...) -> error<TLogger, Args...>;

template <typename TLogger, typename... Args>
error(TLogger*, Tags const& tags, char const*, Args&&...) -> error<TLogger, Args...>;

template <typename TLogger, typename... Args>
struct critical
{
  critical(TLogger* logger, char const* fmt, Args&&... args, SourceLocation location = SourceLocation::current())
  {
    log(logger, "", LogLevel::Critical, fmt, location, std::forward<Args>(args)...);
  }

  critical(TLogger* logger, Tags const& tags, char const* fmt, Args&&... args,
           SourceLocation location = SourceLocation::current())
  {
    log(logger, tags.value(), LogLevel::Critical, fmt, location, std::forward<Args>(args)...);
  }
};

template <typename TLogger, typename... Args>
critical(TLogger*, char const*, Args&&...) -> critical<TLogger, Args...>;

template <typename TLogger, typename... Args>
critical(TLogger*, Tags const& tags, char const*, Args&&...) -> critical<TLogger, Args...>;

template <typename TLogger, typename... Args>
struct backtrace
{
  backtrace(TLogger* logger, char const* fmt, Args&&... args, SourceLocation location = SourceLocation::current())
  {
    log(logger, "", LogLevel::Backtrace, fmt, location, std::forward<Args>(args)...);
  }

  backtrace(TLogger* logger, Tags const& tags, char const* fmt, Args&&... args,
            SourceLocation location = SourceLocation::current())
  {
    log(logger, tags.value(), LogLevel::Backtrace, fmt, location, std::forward<Args>(args)...);
  }
};

template <typename TLogger, typename... Args>
backtrace(TLogger*, char const*, Args&&...) -> backtrace<TLogger, Args...>;

template <typename TLogger, typename... Args>
backtrace(TLogger*, Tags const& tags, char const*, Args&&...) -> backtrace<TLogger, Args...>;

/***/
template <typename TLogger, typename... Args>
QUILL_ATTRIBUTE_HOT void log(TLogger* logger, char const* tags, LogLevel log_level, char const* fmt,
                             SourceLocation const& location, Args&&... args)
{
  if (QUILL_UNLIKELY(!logger))
  {
    return;
  }

  static constexpr MacroMetadata macro_metadata{
    "[placeholder]", "[placeholder]", "[placeholder]",
    nullptr,         LogLevel::None,  MacroMetadata::Event::LogWithRuntimeMetadataHybridCopy};

  if (logger->should_log_statement(log_level))
  {
    logger->template log_statement_runtime_metadata<true>(
      &macro_metadata, fmt, location.file_name(), location.function_name(), tags, location.line(),
      log_level, std::forward<Args>(args)...);
  }
}

QUILL_END_NAMESPACE