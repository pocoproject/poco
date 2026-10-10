/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/LogLevel.h"
#include "quill/core/PatternFormatterOptions.h"
#include "quill/core/QuillError.h"
#include "quill/core/Spinlock.h"
#include "quill/filters/Filter.h"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

QUILL_BEGIN_NAMESPACE

/** Forward Declarations **/
class MacroMetadata;
class PatternFormatter;

namespace detail
{
class BackendWorker;
}

/**
 * Base class for sinks
 */
class Sink
{
public:
  /**
   * Constructor
   * Uses the default pattern formatter
   */
  explicit Sink(std::optional<PatternFormatterOptions> override_pattern_formatter_options = std::nullopt)
    : _override_pattern_formatter_options(std::move(override_pattern_formatter_options))
  {
  }

  /**
   * Destructor
   */
  virtual ~Sink() = default;

  Sink(Sink const&) = delete;
  Sink& operator=(Sink const&) = delete;

  /**
   * @brief Sets a log level filter on the sink.
   * @note Thread safe.
   * @param log_level The log level severity.
   */
  void set_log_level_filter(LogLevel log_level)
  {
    _log_level.store(log_level, std::memory_order_relaxed);
  }

  /**
   * @brief Returns the current log level filter set on the sink.
   * @note Thread safe.
   * @return The current log level filter.
   */
  QUILL_NODISCARD LogLevel get_log_level_filter() const noexcept
  {
    return _log_level.load(std::memory_order_relaxed);
  }

  /**
   * @brief Adds a new filter to the sink.
   * @note Thread safe.
   * @param filter Unique pointer to the filter instance.
   */
  void add_filter(std::unique_ptr<Filter> filter)
  {
    // Lock and add this filter to our global collection
    detail::LockGuard const lock{_global_filters_lock};

    // Check if the same filter already exists
    auto const search_filter_it = std::find_if(
      _global_filters.cbegin(), _global_filters.cend(), [&filter](std::unique_ptr<Filter> const& elem_filter)
      { return elem_filter->get_filter_name() == filter->get_filter_name(); });

    if (QUILL_UNLIKELY(search_filter_it != _global_filters.cend()))
    {
      QUILL_THROW(QuillError{"Filter with the same name already exists"});
    }

    _global_filters.push_back(std::move(filter));

    // Indicate a new filter was added - here relaxed is okay as the spinlock will do acq-rel on destruction
    _new_filter.store(true, std::memory_order_relaxed);
  }

protected:
  /**
   * @brief Logs a formatted log message to the sink.
   * @note Accessor for backend processing.
   * @param log_metadata Pointer to the macro metadata.
   * @param log_timestamp Timestamp of the log event.
   * @param thread_id ID of the thread.
   * @param thread_name Name of the thread.
   * @param process_id Process Id
   * @param logger_name Name of the logger.
   * @param log_level Log level of the message.
   * @param log_level_description Description of the log level.
   * @param log_level_short_code Short code representing the log level.
   * @param named_args Vector of key-value pairs of named args
   * @param log_message The log message.
   * @param log_statement The log statement.
   */
  QUILL_ATTRIBUTE_HOT virtual void write_log(
    MacroMetadata const* log_metadata, uint64_t log_timestamp, std::string_view thread_id,
    std::string_view thread_name, std::string const& process_id, std::string_view logger_name,
    LogLevel log_level, std::string_view log_level_description, std::string_view log_level_short_code,
    std::vector<std::pair<std::string, std::string>> const* named_args,
    std::string_view log_message, std::string_view log_statement) = 0;

  /**
   * @brief Flushes the sink, synchronizing the associated sink with its controlled output sequence.
   */
  QUILL_ATTRIBUTE_HOT virtual void flush_sink() = 0;

  /**
   * @brief Executes periodic tasks by the backend thread, providing an opportunity for the user
   * to perform custom tasks. For example, batch committing to a database, or any other
   * desired periodic operations.
   * @note It is recommended to avoid heavy operations within this function as it may affect performance of the backend thread.
   */
  QUILL_ATTRIBUTE_HOT virtual void run_periodic_tasks() noexcept {}

  /**
   * @brief Applies all registered filters to the log record.
   * @note Called internally by the backend worker thread.
   * @param log_metadata Pointer to the macro metadata.
   * @param log_timestamp Timestamp of the log event.
   * @param thread_id ID of the thread.
   * @param thread_name Name of the thread.
   * @param logger_name Name of the logger.
   * @param log_level Log level of the message.
   * @param log_message The log message.
   * @param log_statement The log message.
   * @return True if the log record passes all filters, false otherwise.
   */
  QUILL_NODISCARD bool apply_all_filters(MacroMetadata const* log_metadata, uint64_t log_timestamp,
                                         std::string_view thread_id, std::string_view thread_name,
                                         std::string_view logger_name, LogLevel log_level,
                                         std::string_view log_message, std::string_view log_statement)
  {
    if (log_level < _log_level.load(std::memory_order_relaxed))
    {
      return false;
    }

    // Update our local collection of the filters
    if (QUILL_UNLIKELY(_new_filter.load(std::memory_order_relaxed)))
    {
      // if there is a new filter we have to update
      _local_filters.clear();

      detail::LockGuard const lock{_global_filters_lock};

      for (auto const& filter : _global_filters)
      {
        _local_filters.push_back(filter.get());
      }

      // all filters loaded so change to false
      _new_filter.store(false, std::memory_order_relaxed);
    }

    if (_local_filters.empty())
    {
      return true;
    }
    else
    {
      return std::all_of(_local_filters.begin(), _local_filters.end(),
                         [log_metadata, log_timestamp, thread_id, thread_name, logger_name,
                          log_level, log_message, log_statement](Filter* filter_elem)
                         {
                           return filter_elem->filter(log_metadata, log_timestamp, thread_id, thread_name,
                                                      logger_name, log_level, log_message, log_statement);
                         });
    }
  }

private:
  friend class detail::BackendWorker;

  /** Override PatternFormatter for this sink **/
  std::optional<PatternFormatterOptions> _override_pattern_formatter_options; /* Set by the frontend and accessed by the backend to initialise PatternFormatter */
  std::shared_ptr<PatternFormatter> _override_pattern_formatter; /* The backend thread will set this once */

  /** Local Filters for this sink **/
  std::vector<Filter*> _local_filters;

  /** Global filter for this sink **/
  std::vector<std::unique_ptr<Filter>> _global_filters;
  detail::Spinlock _global_filters_lock;
  /** Indicator that a new filter was added **/
  std::atomic<bool> _new_filter{false};

  std::atomic<LogLevel> _log_level{LogLevel::TraceL3};
};

QUILL_END_NAMESPACE