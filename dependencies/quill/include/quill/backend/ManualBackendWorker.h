/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/backend/BackendOptions.h"
#include "quill/backend/BackendWorker.h"
#include <chrono>
#include <limits>

QUILL_BEGIN_NAMESPACE

/**
 * This class can be used when you want to run the backend worker on your own thread.
 */
class ManualBackendWorker
{
public:
  explicit ManualBackendWorker(detail::BackendWorker* backend_worker)
    : _backend_worker(backend_worker)
  {
  }

  ~ManualBackendWorker()
  {
    if (_started)
    {
      _backend_worker->_exit();
    }
  }

  /**
   * @brief Initializes the ManualBackendWorker with the specified backend options.
   *
   * This function must be called before any other functions in this class. It configures the backend worker
   * for manual operation, disabling certain options that are incompatible with manual control.
   *
   * @param options The `BackendOptions` to configure the backend worker.
   */
  void init(BackendOptions options)
  {
    options.sleep_duration = std::chrono::nanoseconds{0};
    options.enable_yield_when_idle = false;
    _backend_worker->_init(options);
    _started = true;
  }

  /**
   * Polls all thread-local SPSC queues and caches the log statements, processing and
   * writing the log statement with the minimum timestamp to the corresponding output sinks.
   *
   * This function should be called periodically by the thread to process and dispatch log entries.
   * It assumes that the `init()` function has been called first to properly configure the backend worker.
   *
   */
  void poll_one()
  {
    QUILL_ASSERT(
      _backend_worker->_options.sleep_duration.count() == 0,
      "ManualBackendWorker::poll_one() requires init() to be called first with sleep_duration = 0");
    QUILL_ASSERT(_backend_worker->_options.enable_yield_when_idle == false,
                 "ManualBackendWorker::poll_one() requires init() to be called first with "
                 "enable_yield_when_idle = false");
    QUILL_ASSERT(_backend_worker->_worker_thread_id.load() != 0,
                 "ManualBackendWorker::poll_one() requires init() to be called first");
    QUILL_ASSERT(_backend_worker->_worker_thread_id.load() == detail::get_thread_id(),
                 "ManualBackendWorker::poll_one() must always be called from the same thread");

    QUILL_TRY { _backend_worker->_poll(); }
#if !defined(QUILL_NO_EXCEPTIONS)
    QUILL_CATCH(std::exception const& e) { _backend_worker->_options.error_notifier(e.what()); }
    QUILL_CATCH_ALL()
    {
      _backend_worker->_options.error_notifier(std::string{"Caught unhandled exception."});
    }
#endif
  }

  /**
   * Continuously polls the backend worker until all queues are empty.
   *
   * This function keeps polling until all frontend queues and cached transit events are processed.
   */
  void poll()
  {
    while (!_backend_worker->_check_frontend_queues_and_cached_transit_events_empty())
    {
      poll_one();
    }
  }

  /**
   * This function behaves like `poll()` but will stop polling if the specified timeout duration is reached before
   * all queues are emptied. This can be useful when you want to limit the time spent processing log statements.
   */
  void poll(std::chrono::microseconds timeout)
  {
    auto const start = std::chrono::steady_clock::now();
    while (!_backend_worker->_check_frontend_queues_and_cached_transit_events_empty())
    {
      poll_one();

      auto const now = std::chrono::steady_clock::now();

      if ((now - start) > timeout)
      {
        break;
      }
    }
  }

private:
  detail::BackendWorker* _backend_worker;
  bool _started{false};
};

QUILL_END_NAMESPACE