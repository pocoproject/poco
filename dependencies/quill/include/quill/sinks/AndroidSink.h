/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/LogLevel.h"
#include "quill/sinks/Sink.h"

#include <array>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include <android/log.h>

QUILL_BEGIN_NAMESPACE

/**
 * @brief Holds the configuration options for the AndroidSink.
 *
 * This configuration class allows you to set various parameters that determine how log messages are
 * mapped and sent to the Android log system.
 */
class AndroidSinkConfig
{
public:
  /**
   * @brief Sets the tag that is prepended to every syslog message.
   * Typically, this is set to the program name.
   * @param tag The tag string.
   */
  QUILL_ATTRIBUTE_COLD void set_tag(std::string const& tag) { _tag = tag; }

  /**
   * @brief Enables or disables message formatting.
   * If enabled, the log message will be formatted using the specified
   * PatternFormatter before being sent to syslog.
   * @param format_message Set to true to format the message; false otherwise.
   */
  QUILL_ATTRIBUTE_COLD void set_format_message(bool format_message)
  {
    _format_message = format_message;
  }

  /**
   * @brief Sets the mapping from quill log levels to syslog levels.
   * This mapping determines which syslog level is used for each quill log level.
   * @param mapping An array of 12 integers representing syslog levels.
   */
  QUILL_ATTRIBUTE_COLD void set_log_level_mapping(std::array<int, 12> mapping)
  {
    _log_level_mapping = mapping;
  }

  /** Getters **/
  QUILL_NODISCARD std::string const& tag() const noexcept { return _tag; }
  QUILL_NODISCARD bool should_format_message() const noexcept { return _format_message; }
  QUILL_NODISCARD int get_android_level(quill::LogLevel level) const
  {
    return _log_level_mapping[static_cast<size_t>(level)];
  }

private:
  std::string _tag{"quill"};
  std::array<int, 12> _log_level_mapping = {
    // Mapping from quill log levels to syslog levels:
    /* "TRACE_L3" */ ANDROID_LOG_VERBOSE, /* "TRACE_L2" */ ANDROID_LOG_VERBOSE,
    /* "TRACE_L1" */ ANDROID_LOG_VERBOSE, /* "DEBUG"    */ ANDROID_LOG_DEBUG,
    /* "INFO"     */ ANDROID_LOG_INFO,    /* "NOTICE"   */ ANDROID_LOG_INFO,
    /* "WARNING"  */ ANDROID_LOG_WARN,    /* "ERROR"    */ ANDROID_LOG_ERROR,
    /* "CRITICAL" */ ANDROID_LOG_FATAL,   /* "BACKTRACE"*/ ANDROID_LOG_INFO,
    /* "NONE"     */ ANDROID_LOG_INFO,    /* "DYNAMIC"  */ ANDROID_LOG_INFO};
  bool _format_message{false};
};

/**
 * @brief A sink that writes log messages to the Android logging system (logcat).
 *
 * This sink leverages the Android log API to send log messages. It uses the configuration
 * provided via AndroidSinkConfig to determine how the messages are formatted and mapped.
 */
class AndroidSink : public quill::Sink
{
public:
  /**
   * @brief Constructs an AndroidSink with the given configuration.
   * @param config The configuration options for the AndroidSink. Defaults to a default-configured AndroidSinkConfig.
   */
  explicit AndroidSink(AndroidSinkConfig config = AndroidSinkConfig{}) : _config(std::move(config))
  {
  }

  /***/
  ~AndroidSink() override = default;

  /**
   * @brief Writes a formatted log message to the stream
   */
  void write_log(MacroMetadata const* /* log_metadata */, uint64_t /* log_timestamp */,
                 std::string_view /* thread_id */, std::string_view /* thread_name */,
                 std::string const& /* process_id */, std::string_view /* logger_name */, LogLevel log_level,
                 std::string_view /* log_level_description */, std::string_view /* log_level_short_code */,
                 std::vector<std::pair<std::string, std::string>> const* /* named_args */,
                 std::string_view log_message, std::string_view log_statement) override
  {
    // Choose between formatted log statement or raw log message
    std::string_view const message = _config.should_format_message() ? log_statement : log_message;
    size_t const message_length = message.size();

    __android_log_print(_config.get_android_level(log_level), _config.tag().data(), "%.*s",
                        static_cast<int>(message_length), message.data());
  }

  /***/
  void flush_sink() noexcept override {}

private:
  AndroidSinkConfig _config;
};

QUILL_END_NAMESPACE
