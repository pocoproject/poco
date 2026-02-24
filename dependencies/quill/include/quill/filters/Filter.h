/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/LogLevel.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

QUILL_BEGIN_NAMESPACE

/** Forward Declaration **/
class MacroMetadata;

/**
 * Base filter class.
 * Filters can be added to Sinks
 */
class Filter
{
public:
  /**
   * Constructor
   * @param filter_name unique filter name
   */
  explicit Filter(std::string filter_name) : _filter_name(std::move(filter_name)) {}

  /**
   * Destructor
   */
  virtual ~Filter() = default;

  /**
   * @brief Filters a log message.
   *
   * @param log_metadata Pointer to the macro metadata.
   * @param log_timestamp Timestamp of the log event.
   * @param thread_id ID of the thread.
   * @param thread_name Name of the thread.
   * @param logger_name Name of the logger.
   * @param log_level Log level of the message.
   * @param log_message The log message.
   * @param log_statement The log statement.
   *
   * @return true if the log message should be written to the file, false otherwise
   */
  QUILL_NODISCARD virtual bool filter(MacroMetadata const* log_metadata, uint64_t log_timestamp,
                                      std::string_view thread_id, std::string_view thread_name,
                                      std::string_view logger_name, LogLevel log_level, std::string_view log_message,
                                      std::string_view log_statement) noexcept = 0;

  /**
   * Gets the name of the filter. Only useful if an existing filter is needed to be looked up
   * @return the name of the filter
   */
  QUILL_NODISCARD virtual std::string const& get_filter_name() const noexcept
  {
    return _filter_name;
  }

private:
  std::string _filter_name;
};

QUILL_END_NAMESPACE