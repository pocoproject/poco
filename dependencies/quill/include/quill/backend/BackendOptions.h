/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <limits>
#include <string>

#if defined(__MINGW32__)
  #include <iostream>
#endif

QUILL_BEGIN_NAMESPACE

/**
 * @brief Configuration options for the backend.
 *
 * This struct defines settings for the backend thread
 */
struct BackendOptions
{
  /**
   * The name assigned to the backend, visible during thread naming queries (e.g.,
   * pthread_getname_np) or in the debugger.
   */
  std::string thread_name = "QuillBackend";

  /**
   * The backend employs "busy-waiting" by spinning around each frontend thread's queue.
   * If enabled, the backend will yield when there is no remaining work, potentially
   * reducing the OS scheduler priority for the backend.
   * This option is effective only when sleep_duration is set to 0.
   */
  bool enable_yield_when_idle = false;

  /**
   * Specifies the duration the backend sleeps if there is no remaining work to process in the queues.
   */
  std::chrono::nanoseconds sleep_duration = std::chrono::microseconds{100};

  /**
   * The backend pops all log messages from the frontend queues and buffers them in a
   * local ring buffer queue as transit events. The transit_event_buffer is unbounded, starting with
   * a customizable initial capacity (in items, not bytes) and will reallocate up to
   * transit_events_hard_limit The backend will use a separate transit_event_buffer for each
   * frontend thread. The capacity must be a power of two.
   */
  uint32_t transit_event_buffer_initial_capacity = 256;

  /**
   * The backend gives priority to reading messages from the frontend queues of all
   * the hot threads and temporarily buffers them.
   *
   * If a frontend threads continuously push messages to the queue (e.g., logging in a loop),
   * no logs can ever be processed.
   *
   * When the soft limit is reached the backend worker thread will try to process a batch of cached
   * transit events all at once
   *
   * The frontend queues are emptied on each iteration, so the actual popped messages
   * can be much greater than the transit_events_soft_limit.
   *
   * @note This number represents a limit across the messages received from ALL frontend threads.
   */
  size_t transit_events_soft_limit = 8192;

  /**
   * The backend gives priority to reading messages from the frontend queues and temporarily
   * buffers them.
   *
   * If a frontend thread continuously push messages to the queue (e.g., logging in a loop),
   * no logs can ever be processed.
   *
   * As the backend buffers messages, it can keep buffering indefinitely if the frontend
   * threads keep pushing.
   *
   * This limit is the maximum size of the backend event buffer. When reached, the backend
   * will stop reading the frontend queues until there is space available in the buffer.
   *
   * @note This number represents a limit PER frontend threads.
   */
  size_t transit_events_hard_limit = 65'536;

  /**
   * The backend iterates through all frontend lock-free queues and pops all messages from each
   * queue. It then buffers and logs the message with the lowest timestamp among them.
   *
   * Each frontend lock-free queue corresponds to a thread, and when multiple frontend threads are
   * pushing logs simultaneously, it is possible to read a timestamp from the last queue in the
   * iteration but miss that timestamp when the first queue was read because it was not available at
   * that time.
   *
   * When this option is set to a non-zero value, the backend takes a timestamp (`now()`) before
   * reading the queues. It uses that timestamp to ensure that each log message's timestamp from the
   * frontend queues is less than or equal to the stored `now()` timestamp minus the specified grace
   * period, guaranteeing ordering by timestamp.
   *
   * Messages that fail the above check remain in the lock-free queue. They are checked again in the
   * next iteration. The timestamp check is performed with microsecond precision.
   *
   * Example scenario:
   * 1. Frontend thread takes a timestamp at the very start of logging, then becomes delayed
   *    (preempted, blocked, processing slowly, etc.) before pushing to the queue.
   * 2. Backend thread takes timestamp `now()` and subtracts the grace period, reads queues up to
   *    the adjusted `now()`, and writes the logs.
   * 3. Frontend thread wakes up and pushes the message with its already-recorded timestamp to the queue.
   * 4. Backend thread reads and writes the delayed timestamp, resulting in an out-of-order log.
   *
   * Setting this option to a non-zero value causes a minor delay in reading the messages from the
   * lock-free queues but ensures correct timestamp order.
   *
   * Setting `log_timestamp_ordering_grace_period` to zero disables strict timestamp ordering.
   *
   * - 0μs: Fastest processing, may process messages out of timestamp order
   * - 1-5μs: Good default - minimal delay, occasional reordering possible
   * - 10-20μs: Better ordering when threads have different queue arrival timing
   * - 100μs+: Stricter ordering, but risk of SPSC queue filling up at high throughput logging
   *
   * This compensates for timing differences in when threads push to their queues,
   * not for timestamp accuracy itself.
   */
  std::chrono::microseconds log_timestamp_ordering_grace_period{5};

  /**
   * When this option is enabled and the application is terminating, the backend worker thread
   * will not exit until all the frontend queues are empty.
   *
   * However, if there is a thread during application destruction that keeps trying to log
   * indefinitely, the backend will be unable to exit because it keeps popping log messages.
   *
   * When this option is disabled, the backend will try to read the queues once
   * and then exit. Reading the queues only once means that some log messages can be dropped,
   * especially when strict_log_timestamp_order is set to true.
   */
  bool wait_for_queues_to_empty_before_exit = true;

  /**
   * Pins the backend to the specified CPU.
   *
   * By default, the backend is not pinned to any CPU unless a value is specified.
   * It is recommended to pin the backend to a shared non-critical CPU.
   * Use `std::numeric_limits<uint16_t>::max()` as an undefined value to avoid setting CPU affinity.
   */
  uint16_t cpu_affinity = (std::numeric_limits<uint16_t>::max)();

  /**
   * The backend may encounter exceptions that cannot be caught within user threads.
   * In such cases, the backend invokes this callback to notify the user.
   *
   * This function sets up a user error notifier to handle backend errors and notifications,
   * such as when the unbounded queue reallocates or when the bounded queue becomes full.
   *
   * To disable notifications, simply leave the function undefined:
   *   std::function<void(std::string const&)> backend_error_notifier = {};
   *
   * It's safe to perform logging operations within this function (e.g., LOG_INFO(...)),
   * but avoid calling logger->flush_log(). The function is invoked on the backend thread,
   * which should not remain in a waiting state as it waits for itself.
   */
  std::function<void(std::string const&)> error_notifier = [](std::string const& error_message)
  {
#if !defined(__MINGW32__)
    std::fprintf(stderr, "%s\n", error_message.data());
#else
    // fprintf crashes on mingw gcc 13 for unknown reason
    std::cerr << error_message.data() << "\n";
#endif
  };

  /**
   * Optional hook executed by the backend worker thread at the start of each poll iteration.
   * Any exceptions thrown are forwarded to error_notifier. Useful for thread instrumentation
   * (e.g., Tracy). Use a guard if you need a one-time action.
   */
  std::function<void()> backend_worker_on_poll_begin = {};

  /**
   * Optional hook executed by the backend worker thread at the end of each poll iteration.
   * Any exceptions thrown are forwarded to error_notifier. Useful for thread instrumentation
   * (e.g., Tracy). Use a guard if you need a one-time action.
   */
  std::function<void()> backend_worker_on_poll_end = {};

  /**
   * This option is only applicable if at least one frontend is using a Logger with
   * ClockSourceType::Tsc
   *
   * When the system clock is used, this option can be ignored.
   *
   * Controls the frequency at which the backend recalculates and syncs the internal RdtscClock
   * with the system time from the system wall clock.
   * The TSC clock can drift slightly over time and is not synchronized with NTP server updates.
   *
   * Smaller values provide more accurate log timestamps at the cost of additional system clock
   * calls. Changing this value only affects the performance of the backend worker.
   */
  std::chrono::milliseconds rdtsc_resync_interval = std::chrono::milliseconds{500};

  /**
   * This option specifies the minimum time interval (in milliseconds) before the backend thread
   * flushes the output buffers (flush_sink()) for all sinks in the application.
   *
   * The backend thread will ensure that no sink is flushed more frequently than this interval.
   * Explicit calls to `logger->flush_log()` override this interval and trigger an immediate flush.
   *
   * However, if the backend thread is actively processing messages, flushing may occur less
   * frequently than the specified interval.
   *
   * Setting this value to 0 disables the feature, causing the backend thread to flush sinks
   * whenever there is no pending work, provided a write to the sink has occurred.
   *
   * This setting applies globally and affects all sinks in the application.
   */
  std::chrono::milliseconds sink_min_flush_interval = std::chrono::milliseconds{200};

  /**
   * This option enables a check that verifies the log message contains only printable characters
   * before forwarding it to the sinks. This adds an extra layer of safety by filtering out
   * non-printable characters from the log file. Any non-printable characters are converted to their
   * equivalent hex value.
   *
   * The check applies only when at least one argument in a log statement is of type string.
   *
   * You can customize this callback to define your own range of printable characters if needed.
   *
   * To disable this check, you can provide:
   *   std::function<bool(char c)> check_printable_char = {}
   */
  std::function<bool(char c)> check_printable_char = [](char c)
  { return (c >= ' ' && c <= '~') || (c == '\n') || (c == '\t') || (c == '\r'); };

  /**
   * Holds descriptive names for various log levels used in logging operations.
   * The indices correspond to LogLevel enum values defined elsewhere in the codebase.
   * These names provide human-readable identifiers for each log level.
   */
  std::array<std::string, 11> log_level_descriptions = {
    "TRACE_L3", "TRACE_L2", "TRACE_L1", "DEBUG",     "INFO", "NOTICE",
    "WARNING",  "ERROR",    "CRITICAL", "BACKTRACE", "NONE"};

  /**
   * @brief Short codes or identifiers for each log level.
   *
   * Provides short codes representing each log level for compact identification and usage.
   * The indices correspond to LogLevel enum values defined elsewhere in the codebase.
   */
  std::array<std::string, 11> log_level_short_codes = {"T3", "T2", "T1", "D",  "I", "N",
                                                       "W",  "E",  "C",  "BT", "_"};

  /**
   * Enables a runtime check to detect multiple instances of the backend singleton.
   *
   * When mixing shared and static libraries, linkage issues can lead to multiple instances
   * of the backend singleton. This may result in multiple backend worker threads running
   * simultaneously, causing unexpected behavior or crashes.
   *
   * This issue commonly occurs on Windows when Quill is compiled as a static library and linked
   * into both a shared library and the main executable, creating separate instances. While using
   * Quill as a static library is generally recommended, in such cases, the preferred approach
   * is to build Quill as a shared library and export its symbols
   * (e.g., using `WINDOWS_EXPORT_ALL_SYMBOLS`).
   *
   * On Windows, this check is implemented using a named mutex, whereas on Linux and other POSIX
   * systems, it relies on a named semaphore. In rare cases, this mechanism may interfere with
   * certain environments or containerized deployments. If necessary, this check can be disabled
   * by setting this option to `false`.
   *
   * Setting this option to `true` enables the check, while setting it to `false` disables it.
   */
  bool check_backend_singleton_instance = true;
};

QUILL_END_NAMESPACE