/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#ifndef SD_JOURNAL_SUPPRESS_LOCATION
  #define SD_JOURNAL_SUPPRESS_LOCATION
#endif

#include "quill/core/Attributes.h"
#include "quill/core/LogLevel.h"
#include "quill/core/QuillError.h"
#include "quill/sinks/Sink.h"

#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <systemd/sd-journal.h>
#include <vector>

QUILL_BEGIN_NAMESPACE

/**
 * @note IMPORTANT: Handling Macro Collisions between systemd.h and Quill
 *
 * When including systemd.h via SystemdSink, the header defines macros such as LOG_INFO and others
 * that conflict with Quill's LOG_ macros.
 *
 * To resolve this issue, consider one of the following solutions:
 *
 * 1. **Include SystemdSink in a .cpp file only:**
 *    Instantiate the SystemdSink in a source file rather than a header file. This ensures that
 *    systemd.h is only included in that specific translation unit, avoiding collisions in other
 *    files where you want to use the unprefixed Quill LOG_ macros.
 *
 * 2. **Define the preprocessor flag `QUILL_DISABLE_NON_PREFIXED_MACROS`:**
 *    When this flag is defined, Quill will disable its unprefixed LOG_ macros. Instead, you must
 *    use the longer QUILL_LOG_ macros. This approach allows Quill and systemd.h to coexist in the
 *    same translation unit.
 *
 * Alternatively, you can combine both approaches if you include SystemdSink in a .cpp file where
 * you also intend to use the LOG_ macros.
 *
 * However, the first solution is generally preferable, as it lets you take advantage of the
 * more concise LOG_ macros without additional typing.
 */

/**
 * @brief Holds the configuration options for the SystemdSink.
 *
 * This configuration class allows you to set various parameters that determine how log messages are
 * mapped and sent to the systemd.
 */
class SystemdSinkConfig
{
public:
  /**
   * @brief Sets the identifier that is prepended to every systemd message.
   * Typically, this is set to the program name.
   * @param identifier The identifier string.
   */
  QUILL_ATTRIBUTE_COLD void set_identifier(std::string const& identifier)
  {
    _identifier = identifier;
  }

  /**
   * @brief Enables or disables message formatting.
   * If enabled, the log message will be formatted using the specified
   * PatternFormatter before being sent to systemd.
   * @param format_message Set to true to format the message; false otherwise.
   */
  QUILL_ATTRIBUTE_COLD void set_format_message(bool format_message)
  {
    _format_message = format_message;
  }

  /**
   * @brief Sets the mapping from quill log levels to systemd levels.
   * This mapping determines which systemd level is used for each quill log level.
   * @param mapping An array of 12 integers representing systemd levels.
   */
  QUILL_ATTRIBUTE_COLD void set_log_level_mapping(std::array<int, 12> mapping)
  {
    _log_level_mapping = mapping;
  }

  /** Getters **/
  QUILL_NODISCARD std::string const& identifier() const noexcept { return _identifier; }
  QUILL_NODISCARD bool should_format_message() const noexcept { return _format_message; }
  QUILL_NODISCARD int get_systemd_level(quill::LogLevel level) const
  {
    return _log_level_mapping[static_cast<size_t>(level)];
  }

private:
  std::string _identifier;
  std::array<int, 12> _log_level_mapping = {
    // Mapping from quill log levels to systemd levels:
    /* "TRACE_L3" */ LOG_DEBUG,   /* "TRACE_L2" */ LOG_DEBUG,
    /* "TRACE_L1" */ LOG_DEBUG,   /* "DEBUG"    */ LOG_DEBUG,
    /* "INFO"     */ LOG_INFO,    /* "NOTICE"   */ LOG_NOTICE,
    /* "WARNING"  */ LOG_WARNING, /* "ERROR"    */ LOG_ERR,
    /* "CRITICAL" */ LOG_CRIT,    /* "BACKTRACE"*/ LOG_INFO,
    /* "NONE"     */ LOG_INFO,    /* "DYNAMIC"  */ LOG_INFO};
  bool _format_message{false};
};

/**
 * @brief A sink that writes log messages to the system logger (systemd).
 *
 * This sink leverages the systemd API to send log messages. It uses the configuration
 * provided via SystemdSinkConfig to determine how the messages are formatted and mapped.
 */
class SystemdSink : public quill::Sink
{
public:
  /**
   * @brief Constructs a SystemdSink with the given configuration.
   * This constructor initializes the systemd connection using openlog().
   * @param config The configuration options for the SystemdSink. Defaults to a default-configured SystemdSinkConfig.
   */
  explicit SystemdSink(SystemdSinkConfig config = SystemdSinkConfig{}) : _config(std::move(config))
  {
  }

  /***/
  ~SystemdSink() override { ::closelog(); }

  /**
   * @brief Writes a formatted log message to the stream
   */
  void write_log(MacroMetadata const* log_metadata, uint64_t /* log_timestamp */,
                 std::string_view thread_id, std::string_view /* thread_name */,
                 std::string const& /* process_id */, std::string_view logger_name, LogLevel log_level,
                 std::string_view /* log_level_description */, std::string_view /* log_level_short_code */,
                 std::vector<std::pair<std::string, std::string>> const* /* named_args */,
                 std::string_view log_message, std::string_view log_statement) override
  {
    // Choose between formatted log statement or raw log message
    std::string_view message = _config.should_format_message() ? log_statement : log_message;
    size_t message_length = message.size();

    // Ensure the length does not exceed the maximum int value
    if (message_length > static_cast<size_t>(std::numeric_limits<int>::max()))
    {
      message_length = static_cast<size_t>(std::numeric_limits<int>::max());
    }

    std::string_view identifier = _config.identifier().empty() ? logger_name : _config.identifier();

    // Note: function call inside '()' to avoid macro expansion
    int err =
      (sd_journal_send)("MESSAGE=%.*s", static_cast<int>(message_length), message.data(),
                        "PRIORITY=%d", _config.get_systemd_level(log_level), "TID=%.*s",
                        static_cast<int>(thread_id.size()), thread_id.data(),
                        "SYSLOG_IDENTIFIER=%.*s", static_cast<int>(identifier.size()), identifier.data(),
                        "CODE_FILE=%.*s", static_cast<int>(log_metadata->file_name().size()),
                        log_metadata->file_name().data(), "CODE_LINE=%.*s",
                        static_cast<int>(strlen(log_metadata->line())), log_metadata->line(),
                        "CODE_FUNC=%.*s", static_cast<int>(strlen(log_metadata->caller_function())),
                        log_metadata->caller_function(), nullptr);

    if (QUILL_UNLIKELY(err != 0))
    {
      QUILL_THROW(QuillError{std::string{"Failed writing to systemd with error "} + std::to_string(err)});
    }
  }

  /***/
  void flush_sink() noexcept override {}

private:
  SystemdSinkConfig _config;
};

QUILL_END_NAMESPACE
