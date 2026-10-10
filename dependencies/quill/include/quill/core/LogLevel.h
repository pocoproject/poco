/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/QuillError.h"

#include <cctype>
#include <cstdint>
#include <string>
#include <string_view>

QUILL_BEGIN_NAMESPACE

/**
 * Log level enum
 */
enum class LogLevel : uint8_t
{
  TraceL3,
  TraceL2,
  TraceL1,
  Debug,
  Info,
  Notice,
  Warning,
  Error,
  Critical,
  Backtrace, /**< This is only used for backtrace logging. Should not be set by the user. */
  None
};

namespace detail
{
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT inline std::string_view log_level_to_string(LogLevel log_level,
                                                                                std::string const* log_levels_strings,
                                                                                size_t log_levels_strings_size)
{
  auto const log_lvl = static_cast<uint32_t>(log_level);

  if (QUILL_UNLIKELY(log_lvl >= static_cast<uint32_t>(log_levels_strings_size)))
  {
    std::string const error_msg = "Invalid get_log_level value \"" + std::to_string(log_lvl) + "\"";
    QUILL_THROW(QuillError{error_msg});
  }

  return log_levels_strings[log_lvl];
}
} // namespace detail

/**
 * Converts a string to a LogLevel enum value
 * @param log_level the log level string to convert
 * "tracel3", "tracel2", "tracel1", "debug", "info", "notice", "warning", "error", "critical", "backtrace", "none"
 * @return the corresponding LogLevel enum value
 */
QUILL_NODISCARD LogLevel inline loglevel_from_string(std::string log_level)
{
  // Convert to lowercase
  for (char& c : log_level)
  {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }

  if (log_level == "tracel3" || log_level == "trace_l3")
  {
    return LogLevel::TraceL3;
  }

  if (log_level == "tracel2" || log_level == "trace_l2")
  {
    return LogLevel::TraceL2;
  }

  if (log_level == "tracel1" || log_level == "trace_l1")
  {
    return LogLevel::TraceL1;
  }

  if (log_level == "debug")
  {
    return LogLevel::Debug;
  }

  if (log_level == "info")
  {
    return LogLevel::Info;
  }

  if (log_level == "notice")
  {
    return LogLevel::Notice;
  }

  if (log_level == "warning")
  {
    return LogLevel::Warning;
  }

  if (log_level == "error")
  {
    return LogLevel::Error;
  }

  if (log_level == "critical")
  {
    return LogLevel::Critical;
  }

  if (log_level == "backtrace")
  {
    return LogLevel::Backtrace;
  }

  if (log_level == "none")
  {
    return LogLevel::None;
  }

  std::string const error_msg = "LogLevel enum value does not exist for \"" + log_level + "\"";
  QUILL_THROW(QuillError{error_msg});
}

QUILL_END_NAMESPACE