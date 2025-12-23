/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/backend/BackendOptions.h"
#include "quill/backend/BackendWorker.h"
#include "quill/backend/ManualBackendWorker.h"
#include "quill/core/Attributes.h"

#include <atomic>
#include <cstdint>
#include <mutex>

QUILL_BEGIN_NAMESPACE

// Forward declarations
class Backend;
class BackendTscClock;

namespace detail
{
/**
 * Provides access to common collection class that are used by both the frontend and the backend
 * components of the logging system
 * There should only be only active instance of this class which is achieved by the
 * LogSystemManagerSingleton
 */
class BackendManager
{
public:
  /***/
  QUILL_EXPORT static BackendManager& instance() noexcept
  {
    static BackendManager instance;
    return instance;
  }

  /***/
  BackendManager(BackendManager const&) = delete;
  BackendManager& operator=(BackendManager const&) = delete;

private:
  friend class quill::Backend;
  friend class quill::BackendTscClock;

  /***/
  BackendManager() = default;
  ~BackendManager() { delete _start_once_flag.load(); }

  /***/
  QUILL_ATTRIBUTE_COLD void start_backend_thread(BackendOptions const& options)
  {
    // Start the backend worker
    _backend_worker.run(options);
  }

  /***/
  QUILL_ATTRIBUTE_COLD std::once_flag& get_start_once_flag() noexcept
  {
    return *_start_once_flag.load();
  }

  /***/
  QUILL_ATTRIBUTE_COLD void stop_backend_thread() noexcept
  {
    _backend_worker.stop();

    auto* new_flag = new std::once_flag();
    std::once_flag* old_flag = _start_once_flag.exchange(new_flag);
    delete old_flag;
  }

  /***/
  QUILL_NODISCARD uint32_t get_backend_thread_id() const noexcept
  {
    return _backend_worker.get_backend_thread_id();
  }

  /***/
  void notify_backend_thread() noexcept { _backend_worker.notify(); }

  /***/
  QUILL_NODISCARD bool is_backend_thread_running() const noexcept
  {
    return _backend_worker.is_running();
  }

  /***/
  QUILL_NODISCARD uint64_t convert_rdtsc_to_epoch_time(uint64_t rdtsc_value) const
  {
    return _backend_worker.time_since_epoch(rdtsc_value);
  }

  /***/
  QUILL_NODISCARD ManualBackendWorker* get_manual_backend_worker() noexcept
  {
    return &_manual_backend_worker;
  }

private:
  BackendWorker _backend_worker;
  ManualBackendWorker _manual_backend_worker{&_backend_worker};
  std::atomic<std::once_flag*> _start_once_flag{new std::once_flag};
};
} // namespace detail

QUILL_END_NAMESPACE