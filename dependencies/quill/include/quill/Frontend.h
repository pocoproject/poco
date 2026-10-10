/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/Logger.h"
#include "quill/UserClockSource.h"
#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/FrontendOptions.h"
#include "quill/core/LoggerManager.h"
#include "quill/core/PatternFormatterOptions.h"
#include "quill/core/QuillError.h"
#include "quill/core/SinkManager.h"
#include "quill/core/ThreadContextManager.h"
#include "quill/sinks/Sink.h"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <thread>
#include <vector>

QUILL_BEGIN_NAMESPACE

template <typename TFrontendOptions>
class FrontendImpl
{
public:
  using frontend_options_t = TFrontendOptions;
  using logger_t = LoggerImpl<frontend_options_t>;

  /**
   * @brief Pre-allocates the thread-local data needed for the current thread.
   *
   * Although optional, it is recommended to invoke this function during the thread initialization
   * phase before the first log message.
   */
  QUILL_ATTRIBUTE_COLD static void preallocate()
  {
    auto const volatile spsc_queue_capacity = detail::get_local_thread_context<TFrontendOptions>()
                                                ->template get_spsc_queue<TFrontendOptions::queue_type>()
                                                .capacity();

    // On windows and c++17, QUILL_MAYBE_UNUSED won't work
    (void)spsc_queue_capacity;
  }

  /**
   * Shrink the thread-local SPSC queue to the specified target capacity.
   *
   * This function helps manage memory usage by reducing the size of the thread-local queue.
   * In scenarios where a thread pool executes multiple jobs, one job might log a burst of messages
   * that causes the queue to grow significantly. Subsequent jobs may not require such a large capacity,
   * so you can call this function to explicitly shrink the queue to a smaller size.
   *
   * @note This function only applies when using the **UnboundedQueue** configuration. It will have no effect
   *       if the BoundedQueue is enabled.
   * @note The function will only shrink the queue if the provided target capacity is smaller than the current
   *       queue capacity. If the target capacity is greater than or equal to the current capacity, no change is made.
   * @warning The Logger object may maintain multiple thread-local queues. This function will only shrink the queue
   *          associated with the calling thread, so it is important that the appropriate thread invokes it.
   *
   * @param capacity The desired new capacity for the thread-local SPSC queue.
   */
  static void shrink_thread_local_queue(size_t capacity)
  {
    if constexpr (logger_t::using_unbounded_queue)
    {
      detail::get_local_thread_context<TFrontendOptions>()
        ->template get_spsc_queue<TFrontendOptions::queue_type>()
        .shrink(capacity);
    }
  }

  /**
   * Retrieve the current capacity of the thread-local SPSC queue.
   *
   * This function returns the capacity of the SPSC queue that belongs to the calling thread.
   * It is particularly useful for monitoring how much an UnboundedQueue has grown over time,
   * while for a BoundedQueue, the capacity remains constant.
   *
   * @note When using an UnboundedQueue, the function returns the capacity as determined by the producer,
   *       reflecting the dynamic growth of the queue. For a BoundedQueue, the returned capacity is fixed.
   * @note Since the Logger object can maintain multiple thread-local queues, this function always returns
   *       the capacity of the queue associated with the thread that calls it. Ensure that the correct thread
   *       is invoking this function to check its own queue.
   *
   * @return The current capacity of the thread-local SPSC queue.
   */
  QUILL_NODISCARD static size_t get_thread_local_queue_capacity() noexcept
  {
    if constexpr (logger_t::using_unbounded_queue)
    {
      return detail::get_local_thread_context<TFrontendOptions>()
        ->template get_spsc_queue<TFrontendOptions::queue_type>()
        .producer_capacity();
    }
    else
    {
      return detail::get_local_thread_context<TFrontendOptions>()
        ->template get_spsc_queue<TFrontendOptions::queue_type>()
        .capacity();
    }
  }

  /**
   * @brief Creates a new sink or retrieves an existing one with the specified name.
   *
   * @param sink_name The name of the sink.
   * @param args The arguments to pass to the sink constructor.
   * @return std::shared_ptr<Sink> A shared pointer to the created or retrieved sink.
   */
  template <typename TSink, typename... Args>
  static std::shared_ptr<Sink> create_or_get_sink(std::string const& sink_name, Args&&... args)
  {
    return detail::SinkManager::instance().create_or_get_sink<TSink>(sink_name, static_cast<Args&&>(args)...);
  }

  /**
   * @brief Retrieves an existing sink with the specified name.
   *
   * @param sink_name The name of the sink.
   * @return std::shared_ptr<Sink> A shared pointer to the retrieved sink, or nullptr if not found.
   */
  QUILL_NODISCARD static std::shared_ptr<Sink> get_sink(std::string const& sink_name)
  {
    return detail::SinkManager::instance().get_sink(sink_name);
  }

  /**
   * @brief Creates a new logger or retrieves an existing one with the specified name.
   *
   * @param logger_name The name of the logger.
   * @param sink A shared pointer to the sink to associate with the logger.
   * @param pattern_formatter_options Contains the formatting configuration for PatternFormatter
   * @param clock_source The clock source for log timestamps.
   * @param user_clock A pointer to a custom user clock.
   *
   * @return Logger* A pointer to the created or retrieved logger.
   */
  static logger_t* create_or_get_logger(std::string const& logger_name, std::shared_ptr<Sink> sink,
                                        PatternFormatterOptions const& pattern_formatter_options = PatternFormatterOptions{},
                                        ClockSourceType clock_source = ClockSourceType::Tsc,
                                        UserClockSource* user_clock = nullptr)
  {
    std::vector<std::shared_ptr<Sink>> sinks;
    sinks.push_back(static_cast<std::shared_ptr<Sink>&&>(sink));

    return _cast_to_logger(detail::LoggerManager::instance().create_or_get_logger<logger_t>(
      logger_name, static_cast<std::vector<std::shared_ptr<Sink>>&&>(sinks),
      pattern_formatter_options, clock_source, user_clock));
  }

  /**
   * @brief Creates a new logger or retrieves an existing one with the specified name and multiple sinks.
   *
   * @param logger_name The name of the logger.
   * @param sinks A vector of shared pointers to sinks to associate with the logger.
   * @param pattern_formatter_options Contains the formatting configuration for PatternFormatter
   * @param clock_source The clock source for log timestamps.
   * @param user_clock A pointer to a custom user clock.
   * @return Logger* A pointer to the created or retrieved logger.
   */
  static logger_t* create_or_get_logger(
    std::string const& logger_name, std::vector<std::shared_ptr<Sink>> sinks,
    PatternFormatterOptions const& pattern_formatter_options = PatternFormatterOptions{},
    ClockSourceType clock_source = ClockSourceType::Tsc, UserClockSource* user_clock = nullptr)
  {
    return _cast_to_logger(detail::LoggerManager::instance().create_or_get_logger<logger_t>(
      logger_name, static_cast<std::vector<std::shared_ptr<Sink>>&&>(sinks),
      pattern_formatter_options, clock_source, user_clock));
  }

  /**
   * @brief Creates a new logger or retrieves an existing one with the specified name and multiple sinks.
   *
   * @param logger_name The name of the logger.
   * @param sinks An initializer list of shared pointers to sinks to associate with the logger.
   * @param pattern_formatter_options Contains the formatting configuration for PatternFormatter
   * @param clock_source The clock source for log timestamps.
   * @param user_clock A pointer to a custom user clock.
   * @return Logger* A pointer to the created or retrieved logger.
   */
  static logger_t* create_or_get_logger(
    std::string const& logger_name, std::initializer_list<std::shared_ptr<Sink>> sinks,
    PatternFormatterOptions const& pattern_formatter_options = PatternFormatterOptions{},
    ClockSourceType clock_source = ClockSourceType::Tsc, UserClockSource* user_clock = nullptr)
  {
    return create_or_get_logger(logger_name, std::vector<std::shared_ptr<Sink>>{sinks},
                                pattern_formatter_options, clock_source, user_clock);
  }

  /**
   * @brief Creates a new logger or retrieves an existing one that shares the same options as the specified logger.
   *
   * This function allows you to create or obtain a logger identified by `logger_name`. If a logger with the
   * same name already exists, its configuration options will be used. If it does not exist, a new logger
   * will be created with the same options as the provided `source_logger`.
   *
   * @param logger_name The name of the logger to create or retrieve.
   * @param source_logger The logger from which to copy the configuration options.
   * @return A pointer to the logger instance, either newly created or retrieved.
   */
  static logger_t* create_or_get_logger(std::string const& logger_name, detail::LoggerBase* source_logger = nullptr)
  {
    return _cast_to_logger(
      detail::LoggerManager::instance().create_or_get_logger<logger_t>(logger_name, source_logger));
  }

  /**
   * @brief Asynchronously removes the specified logger.
   *
   * When a logger is removed, if its underlying sinks are not shared by any other logger,
   * they are destructed, and any associated files are also closed.
   *
   * A common use case for this function is when you want to close the underlying files that the logger is using.
   *
   * Since the exact removal timing is unknown, you should not attempt to create a new logger
   * with the same name as the one being removed.
   *
   * @note This function is thread-safe. However, removing the same logger (`logger_t*`) from multiple threads is not allowed. You must ensure that remove_logger_blocking is only called by a single thread for a given logger.
   * @warning After calling this function, no thread should use this logger.
   *
   * @param logger A pointer to the logger to remove.
   */
  static void remove_logger(detail::LoggerBase* logger)
  {
    detail::LoggerManager::instance().remove_logger(logger);
  }

  /**
   * @brief Asynchronously removes the specified logger and blocks until removal is complete.
   *
   * When a logger is removed, any files associated with its sinks are also closed.
   *
   * A use case for this function is when you want to change the sinks of a logger.
   * You can call this function to remove the logger and then recreate it with new sinks and the same name.
   * However, no other threads should be using the logger after the call to this function.
   *
   * @note This function is thread-safe. However, removing the same logger (`logger_t*`) from multiple threads is not allowed. You must ensure that remove_logger_blocking is only called by a single thread for a given logger.
   * @warning After calling this function, no thread should use this logger.
   *
   * @param logger A pointer to the logger to remove.
   * @param sleep_duration_ns The duration in nanoseconds to sleep between retries
   */
  static void remove_logger_blocking(logger_t* logger, uint32_t sleep_duration_ns = 100)
  {
    static constexpr MacroMetadata macro_metadata{
      "", "", "", nullptr, LogLevel::Critical, MacroMetadata::Event::LoggerRemovalRequest};

    std::atomic<bool> logger_removal_complete{false};
    std::atomic<bool>* logger_removal_complete_ptr = &logger_removal_complete;

    while (!logger->template log_statement<false>(
      &macro_metadata, reinterpret_cast<uintptr_t>(logger_removal_complete_ptr), logger->get_logger_name()))
    {
      // We do not want to drop the message if a dropping queue is used
      if (sleep_duration_ns > 0)
      {
        std::this_thread::sleep_for(std::chrono::nanoseconds{sleep_duration_ns});
      }
      else
      {
        std::this_thread::yield();
      }
    }

    detail::LoggerManager::instance().remove_logger(logger);

    while (!logger_removal_complete.load())
    {
      // The caller thread keeps checking the flag until the logger is removed
      if (sleep_duration_ns > 0)
      {
        std::this_thread::sleep_for(std::chrono::nanoseconds{sleep_duration_ns});
      }
      else
      {
        std::this_thread::yield();
      }
    }
  }

  /**
   * @brief Retrieves an existing logger with the specified name.
   *
   * @param logger_name The name of the logger.
   * @return Logger* A pointer to the retrieved logger, or nullptr if not found.
   */
  QUILL_NODISCARD static logger_t* get_logger(std::string const& logger_name)
  {
    detail::LoggerBase* logger = detail::LoggerManager::instance().get_logger(logger_name);
    return logger ? _cast_to_logger(logger) : nullptr;
  }

  /**
   * @brief Retrieves a map of all registered valid loggers.
   * @note If `remove_logger()` is called from this or another thread, the return value
   *       of this function will become invalid.
   * @return A vector containing all registered loggers.
   */
  QUILL_NODISCARD static std::vector<logger_t*> get_all_loggers()
  {
    std::vector<detail::LoggerBase*> logger_bases = detail::LoggerManager::instance().get_all_loggers();

    std::vector<logger_t*> loggers;

    for (auto const& logger_base : logger_bases)
    {
      loggers.push_back(_cast_to_logger(logger_base));
    }

    return loggers;
  }

  /**
   * Returns the first valid logger that is found. This is useful when you do not want to use the
   * std::vector<logger_t*> return value of get_all_loggers.
   *
   * @return A pointer to the first valid logger, or nullptr if no valid logger is found.
   */
  QUILL_NODISCARD static logger_t* get_valid_logger() noexcept
  {
    detail::LoggerBase* logger = detail::LoggerManager::instance().get_valid_logger();
    return logger ? _cast_to_logger(logger) : nullptr;
  }

  /**
   * @brief Counts the number of existing loggers, including any invalidated loggers.
   * This function can be useful for verifying if a logger has been removed after calling
   * remove_logger() by the backend, as removal occurs asynchronously.
   * @return The number of loggers.
   */
  QUILL_NODISCARD static size_t get_number_of_loggers() noexcept
  {
    return detail::LoggerManager::instance().get_number_of_loggers();
  }

private:
  QUILL_NODISCARD static logger_t* _cast_to_logger(detail::LoggerBase* logger_base)
  {
    QUILL_ASSERT(logger_base, "logger_base is nullptr in Frontend::_cast_to_logger()");

#if QUILL_USE_RTTI
    // MSVC may warn that a variable with the same name is hidden by this declaration,
    // so the variable is prefixed with an underscore ('_') to avoid shadowing.
    auto* _logger_cast = dynamic_cast<logger_t*>(logger_base);

    if (QUILL_UNLIKELY(!_logger_cast))
    {
      QUILL_THROW(QuillError{"Failed to cast logger. Invalid logger type."});
    }
#else
    // MSVC may warn that a variable with the same name is hidden by this declaration,
    // so the variable is prefixed with an underscore ('_') to avoid shadowing.
    auto* _logger_cast = static_cast<logger_t*>(logger_base);
#endif

    return _logger_cast;
  }
};

using Frontend = FrontendImpl<FrontendOptions>;

QUILL_END_NAMESPACE
