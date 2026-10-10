/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#if defined(_WIN32)
  #if !defined(WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
  #endif

  #if !defined(NOMINMAX)
    // Mingw already defines this, so no need to redefine
    #define NOMINMAX
  #endif

  #include <windows.h>
#else
  #include <cstring>
  #include <fcntl.h> // For O_CREAT and O_EXCL
  #include <semaphore.h>
#endif

#include "quill/core/Attributes.h"
#include "quill/core/QuillError.h"

#include <cstring>
#include <string>

QUILL_BEGIN_NAMESPACE

namespace detail
{
/**
 * @brief Ensures that only one instance of the backend worker is active.
 *
 * This class provides a mechanism to prevent multiple instances of the backend worker
 * from running within the same process. This can occur due to inconsistent linkage
 * (e.g., mixing static and shared libraries), potentially leading to duplicate backend
 * worker threads and unexpected behavior.
 *
 * On Windows, it utilizes a named mutex, while on POSIX systems, it uses a named semaphore.
 */
class BackendWorkerLock
{
public:
  /***/
  explicit BackendWorkerLock(std::string const& pid)
  {
#if defined(_WIN32)
    std::string name = "Local\\QuillLock" + pid;

    // Create a named mutex. If it already exists in this process, the flag ERROR_ALREADY_EXISTS is set.
    _handle = CreateMutexA(nullptr, TRUE, name.data());

    if (_handle == nullptr)
    {
      QUILL_THROW(QuillError{"Failed to create mutex"});
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
      // Another instance in the same process already holds the lock.
      QUILL_THROW(QuillError{
        "Duplicate backend worker thread detected. This indicates that the logging library has "
        "been compiled into multiple binary modules (for instance, one module using a static build "
        "and another using a shared build), resulting in separate instances of the backend worker. "
        "Please build and link the logging library uniformly as a shared library with exported "
        "symbols to ensure a single backend instance."});
    }
#elif defined(__ANDROID__)
    // disabled
#else
    std::string name = "/QuillLock" + pid;

    // Open or create the named semaphore.
    // O_CREAT will create the semaphore if it doesn't exist, and if it does exist it will simply open the same semaphore.
    _sem = sem_open(name.data(), O_CREAT, 0644, 1);
    if (_sem == SEM_FAILED)
    {
      QUILL_THROW(QuillError{"Failed to create semaphore - errno: " + std::to_string(errno) +
                             " error: " + std::strerror(errno)});
    }

    // Immediately unlink it so that it leaves no traces.
    // The semaphore will still exist until all descriptors are closed.
    sem_unlink(name.data());

    // Try to lock the semaphore.
    // If it’s already locked (by another instance within the same process),
    // sem_trywait will return -1 and set errno.
    if (sem_trywait(_sem) != 0)
    {
      QUILL_THROW(QuillError{
        "Duplicate backend worker thread detected. This indicates that the logging library has "
        "been compiled into multiple binary modules (for instance, one module using a static build "
        "and another using a shared build), resulting in separate instances of the backend worker. "
        "Please build and link the logging library uniformly as a shared library with exported "
        "symbols to ensure a single backend instance."});
    }
#endif
  }

  /***/
  ~BackendWorkerLock()
  {
#if defined(_WIN32)
    if (_handle != nullptr)
    {
      ReleaseMutex(_handle);
      CloseHandle(_handle);
      _handle = nullptr;
    }
#else
    if (_sem != SEM_FAILED)
    {
      sem_post(_sem);
      sem_close(_sem);
      _sem = SEM_FAILED;
    }
#endif
  }

  // Disable copy and assignment.
  BackendWorkerLock(BackendWorkerLock const&) = delete;
  BackendWorkerLock& operator=(BackendWorkerLock const&) = delete;

private:
#if defined(_WIN32)
  HANDLE _handle{nullptr};
#else
  sem_t* _sem{SEM_FAILED};
#endif
};
} // namespace detail

QUILL_END_NAMESPACE