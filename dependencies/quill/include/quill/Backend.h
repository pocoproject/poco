/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/backend/BackendManager.h"
#include "quill/backend/BackendOptions.h"
#include "quill/backend/SignalHandler.h"
#include "quill/core/Attributes.h"
#include "quill/core/QuillError.h"

#include <atomic>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <mutex>

QUILL_BEGIN_NAMESPACE

/** Version Info - When updating VersionMajor please also update the namespace in Attributes.h **/
constexpr uint32_t VersionMajor{11};
constexpr uint32_t VersionMinor{1};
constexpr uint32_t VersionPatch{0};
constexpr uint32_t Version{VersionMajor * 10000 + VersionMinor * 100 + VersionPatch};

class Backend
{
public:
  /**
   * Starts the backend thread.
   * @param options Backend options to configure the backend behavior.
   */
  QUILL_ATTRIBUTE_COLD static void start(BackendOptions const& options = BackendOptions{})
  {
    std::call_once(detail::BackendManager::instance().get_start_once_flag(),
                   [options]()
                   {
                     // Run the backend worker thread, we wait here until the thread enters the main loop
                     detail::BackendManager::instance().start_backend_thread(options);

                     // Set up an exit handler to call stop when the main application exits.
                     // always call stop on destruction to log everything. std::atexit seems to be
                     // working better with dll on windows compared to using ~LogManagerSingleton().
                     std::atexit([]() { detail::BackendManager::instance().stop_backend_thread(); });
                   });
  }
  /**
   * Starts the backend thread and initialises a signal handler
   *
   * @param backend_options Backend options to configure the backend behavior.
   * @param signal_handler_options SignalHandler options to configure the signal handler behavior.
   * is enabled.
   *
   * @note When using the SignalHandler on Linux/MacOS, ensure that each spawned thread in your
   * application has performed one of the following actions:
   * i) Logged at least once.
   * or ii) Called Frontend::preallocate().
   * or iii) Blocked signals on that thread to prevent the signal handler from running on it.
   * This requirement is because the built-in signal handler utilizes a lock-free queue to issue log
   * statements and await the log flushing. The queue is constructed on its first use with `new()`.
   * Failing to meet any of the above criteria means the queue was never used, and it will be
   * constructed inside the signal handler. The `new` operation is not an async signal-safe function
   * and may lead to potential issues. However, when the queue is already created, no `new` call is
   * made, and the remaining functions invoked internally by the built-in signal handler are async
   * safe.
   */
  template <typename TFrontendOptions>
  QUILL_ATTRIBUTE_COLD static void start(BackendOptions const& backend_options,
                                         SignalHandlerOptions const& signal_handler_options)
  {
    std::call_once(detail::BackendManager::instance().get_start_once_flag(),
                   [backend_options, signal_handler_options]()
                   {
#if defined(_WIN32)
                     detail::init_exception_handler<TFrontendOptions>();
#else
        // We do not want signal handler to run in the backend worker thread
        // Block signals in the main thread so when we spawn the backend worker thread it inherits
        // the master
        sigset_t set, oldset;
        sigfillset(&set);
        sigprocmask(SIG_SETMASK, &set, &oldset);
        detail::init_signal_handler<TFrontendOptions>(signal_handler_options.catchable_signals);
#endif

                     // Run the backend worker thread, we wait here until the thread enters the main loop
                     detail::BackendManager::instance().start_backend_thread(backend_options);

                     detail::SignalHandlerContext::instance().logger_name = signal_handler_options.logger_name;

                     detail::SignalHandlerContext::instance().excluded_logger_substrings =
                       signal_handler_options.excluded_logger_substrings;

                     detail::SignalHandlerContext::instance().signal_handler_timeout_seconds.store(
                       signal_handler_options.timeout_seconds);

                     // We need to update the signal handler with some backend thread details
                     detail::SignalHandlerContext::instance().backend_thread_id.store(
                       detail::BackendManager::instance().get_backend_thread_id());

#if defined(_WIN32)
      // nothing to do
#else
                     // Unblock signals in the main thread so subsequent threads do not inherit the blocked mask
                     sigprocmask(SIG_SETMASK, &oldset, nullptr);
#endif

                     // Set up an exit handler to call stop when the main application exits.
                     // always call stop on destruction to log everything. std::atexit seems to be
                     // working better with dll on windows compared to using ~LogManagerSingleton().
                     std::atexit([]() { detail::BackendManager::instance().stop_backend_thread(); });
                   });
  }

  /**
   * Stops the backend thread.
   * @note thread-safe
   */
  QUILL_ATTRIBUTE_COLD static void stop() noexcept
  {
    detail::BackendManager::instance().stop_backend_thread();
  }

  /**
   * Notifies the backend thread to wake up.
   * It is possible to use a long backend sleep_duration and then notify the backend to wake up
   * from any frontend thread.
   *
   * @note thread-safe
   */
  static void notify() noexcept { detail::BackendManager::instance().notify_backend_thread(); }

  /**
   * Checks if the backend is currently running.
   * @return True if the backend is running, false otherwise.
   */
  QUILL_NODISCARD static bool is_running() noexcept
  {
    return detail::BackendManager::instance().is_backend_thread_running();
  }

  /**
   * Retrieves the ID of the backend thread.
   * @return The ID of the backend thread.
   */
  QUILL_NODISCARD static uint32_t get_thread_id() noexcept
  {
    return detail::BackendManager::instance().get_backend_thread_id();
  }

  /**
   * Converts an rdtsc value to epoch time.
   * This function uses the same clock as the backend and can be called from any frontend thread.
   * It is useful when using a logger with ClockSourceType::Tsc and you want to obtain a timestamp
   * synchronized with the log files generated by the backend.
   *
   * Alternatively you can use the Clock class from backend/Clock.h
   * @param rdtsc_value The RDTSC value to convert.
   * @return The epoch time corresponding to the RDTSC value.
   */
  QUILL_NODISCARD static uint64_t convert_rdtsc_to_epoch_time(uint64_t rdtsc_value) noexcept
  {
    return detail::BackendManager::instance().convert_rdtsc_to_epoch_time(rdtsc_value);
  }

  /**
   * This feature is designed for advanced users who need to run the backend worker on their own thread,
   * providing more flexibility at the cost of complexity and potential pitfalls.
   *
   * This approach is generally not recommended due to the potential for inefficiency and complexity
   * in managing the backend worker outside the provided mechanisms.
   *
   * Important notes:
   *   - Do not use this to run the library in a single threaded application. This will lead to inefficiencies.
   *     The design of this logging library assumes that the backend worker operates in a separate thread from the frontend threads that issue log statements.
   *   - The thread running the `ManualBackendWorker` can log but it must not call `logger->flush_log()`, as this can
   *     lead to a deadlock situation.
   *   - The `ManualBackendWorker` should only be used by a single thread. It is not designed to handle
   *     multiple threads calling `poll()` simultaneously.
   *   - The built-in signal handler is not set up with `ManualBackendWorker`. If signal handling is
   *     required, you must manually set up the signal handler and block signals from reaching the `ManualBackendWorker` thread.
   *     See the `start<FrontendOptions>(BackendOptions, SignalHandlerOptions)` implementation for guidance on how to do this.
   *   - The following options are not supported when using `ManualBackendWorker`: `cpu_affinity`,
   *     `thread_name`, `sleep_duration`, and `enable_yield_when_idle`.
   *   - Avoid performing very heavy tasks in your custom thread. Significant delays in calling `poll()`
   *     can lead to the SPSC queues of the frontend threads becoming full. When this happens, the
   *     frontend threads may need to allocate additional memory on the hot path.
   *
   * @code
   * std::thread backend_worker([]()
   *   {
   *     quill::ManualBackendWorker* manual_backend_worker = quill::Backend::acquire_manual_backend_worker();
   *
   *     quill::BackendOptions backend_options;
   *     manual_backend_worker->init(backend_options);
   *
   *     while (true)
   *     {
   *       manual_backend_worker->poll();
   *     }
   *   });
   * @endcode
   */
  QUILL_ATTRIBUTE_COLD static ManualBackendWorker* acquire_manual_backend_worker()
  {
    ManualBackendWorker* manual_backend_worker{nullptr};

    std::call_once(
      detail::BackendManager::instance().get_start_once_flag(), [&manual_backend_worker]() mutable
      { manual_backend_worker = detail::BackendManager::instance().get_manual_backend_worker(); });

    if (!manual_backend_worker)
    {
      QUILL_THROW(
        QuillError{"acquire_manual_backend_worker() can only be called once per process. "
                   "Additionally, it should not be "
                   "called when start() has already been invoked"});
    }

    return manual_backend_worker;
  }
};

QUILL_END_NAMESPACE