/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Common.h"

#include <limits>
#include <string>

QUILL_BEGIN_NAMESPACE

/**
 * @brief Configuration options for the PatternFormatter.
 *
 * This class encapsulates the configuration options used to customize
 * the formatting of log messages.
 */
class PatternFormatterOptions
{
public:
  /***/
  explicit PatternFormatterOptions(std::string format_pattern =
                                     "%(time) [%(thread_id)] %(short_source_location:<28) "
                                     "LOG_%(log_level:<9) %(logger:<12) %(message)",
                                   std::string timestamp_pattern = "%H:%M:%S.%Qns",
                                   Timezone timestamp_timezone = Timezone::LocalTime,
                                   bool add_metadata_to_multi_line_logs = true,
                                   char pattern_suffix = '\n')
    : format_pattern(static_cast<std::string&&>(format_pattern)),
      timestamp_pattern(static_cast<std::string&&>(timestamp_pattern)),
      timestamp_timezone(timestamp_timezone),
      add_metadata_to_multi_line_logs(add_metadata_to_multi_line_logs),
      pattern_suffix(pattern_suffix)
  {
  }

  /**
   * @brief The format pattern for log messages.
   *
   * This string defines the overall structure of each log message.
   *
   * It can include various placeholders that will be replaced with actual values when formatting the log message.
   *
   * %(time)                    - Human-readable timestamp representing when the log statement was created.
   * %(file_name)               - Name of the source file where the logging call was issued.
   * %(full_path)               - Full path of the source file where the logging call was issued.
   * %(caller_function)         - Name of the function containing the logging call.
   * %(log_level)               - Textual representation of the logging level for the message.
   * %(log_level_short_code)    - Abbreviated log level name.
   * %(line_number)             - Line number in the source file where the logging call was issued.
   * %(logger)                  - Name of the logger used to log the call.
   * %(message)                 - The logged message itself.
   * %(thread_id)               - ID of the thread in which the logging call was made.
   * %(thread_name)             - Name of the thread. Must be set before the first log statement on that thread.
   * %(process_id)              - ID of the process in which the logging call was made.
   * %(source_location)         - Full source file path and line number as a single string.
   * %(short_source_location)   - Shortened source file name and line number as a single string.
   * %(tags)                    - Additional custom tags appended to the message when _TAGS macros are used.
   * %(named_args)              - Key-value pairs appended to the message. Only applicable when the message has named args; remains empty otherwise.
   *
   * @warning The same attribute cannot be used twice in the same format pattern.
   */
  std::string format_pattern{
    "%(time) [%(thread_id)] %(short_source_location:<28) LOG_%(log_level:<9) %(logger:<12) "
    "%(message)"};

  /**
   * @brief The format pattern for timestamps.
   *
   * This string defines how timestamps are formatted in log messages.
   * It follows the strftime() format with additional specifiers:
   * - %Qms : Milliseconds
   * - %Qus : Microseconds
   * - %Qns : Nanoseconds
   */
  std::string timestamp_pattern{"%H:%M:%S.%Qns"};

  /**
   * @brief Sets a path prefix to be stripped from source location paths.
   *
   * When set, any source location paths that start with this prefix will have the prefix removed:
   * - For example, with prefix "/home/user/", a path like "/home/user/project/test.cpp:5"
   *   would be displayed as "project/test.cpp:5"
   * - If empty (default), the full path is shown
   *
   * This affects only the %(source_location) attribute.
   */
  std::string source_location_path_strip_prefix{};

  /**
   * @brief Function pointer for custom processing of detailed function names for %(caller_function)
   *
   * This is most useful when QUILL_DETAILED_FUNCTION_NAME is enabled, as it allows
   * custom processing of the detailed function signature provided by the compiler.
   *
   * Since the format of __PRETTY_FUNCTION__ or equivalent is compiler-specific,
   * this function allows users to implement their own parsing/formatting logic.
   *
   * The function takes one parameter:
   * - The raw function signature string from the compiler (e.g., from __PRETTY_FUNCTION__)
   *
   * It should return a string_view representing the processed function name.
   *
   * If set to nullptr (default), the logger will use the unprocessed function name
   * as provided by the compiler.
   */
  std::string_view (*process_function_name)(char const*){nullptr};

  /**
   * @brief The timezone to use for timestamps.
   *
   * Determines whether timestamps are formatted in local time or GMT.
   */
  Timezone timestamp_timezone{Timezone::LocalTime};

  /**
   * @brief Whether to add metadata to each line of multi-line log messages.
   *
   * If true, ensures that metadata (e.g., timestamp, log level) is added
   * to every line of multi-line log entries, maintaining consistency
   * across all log outputs.
   */
  bool add_metadata_to_multi_line_logs{true};

  /**
   * @brief Whether to remove relative path components from source location paths.
   *
   * If true, relative path components like "../" will be processed and removed
   * from source location paths, simplifying the displayed path.
   *
   * This affects only the %(source_location) attribute.
   */
  bool source_location_remove_relative_paths{false};

  /**
   * @brief Character to append at the end of each formatted log pattern.
   *
   * This character is appended to the formatted log message pattern.
   * - If set to a character (e.g., '\n'), that character will be appended
   * - If set to NO_SUFFIX, no character will be appended
   */
  char pattern_suffix{'\n'};

  /**
   * @brief Special value to indicate no pattern suffix should be appended
   * Using -1 cast to char ensures this value is unlikely to conflict with legitimate suffix characters
   */
  static constexpr char NO_SUFFIX = static_cast<char>(-1);

  /***/
  bool operator==(PatternFormatterOptions const& other) const noexcept
  {
    return format_pattern == other.format_pattern && timestamp_pattern == other.timestamp_pattern &&
      source_location_path_strip_prefix == other.source_location_path_strip_prefix &&
      timestamp_timezone == other.timestamp_timezone && process_function_name == other.process_function_name &&
      add_metadata_to_multi_line_logs == other.add_metadata_to_multi_line_logs &&
      source_location_remove_relative_paths == other.source_location_remove_relative_paths &&
      pattern_suffix == other.pattern_suffix;
  }

  /***/
  bool operator!=(PatternFormatterOptions const& other) const noexcept { return !(*this == other); }
};

QUILL_END_NAMESPACE