/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include <atomic>
#include <cstdint>

QUILL_BEGIN_NAMESPACE

namespace detail
{
/***/
class Spinlock
{
public:
  Spinlock() = default;

  /**
   * Deleted
   */
  Spinlock(Spinlock const&) = delete;
  Spinlock& operator=(Spinlock const&) = delete;

  /***/
  QUILL_ATTRIBUTE_HOT void lock() noexcept
  {
    do
    {
      while (_flag.load(std::memory_order_relaxed) == State::Locked)
      {
        // keep trying
      }
    } while (_flag.exchange(State::Locked, std::memory_order_acquire) == State::Locked);
  }

  /***/
  QUILL_ATTRIBUTE_HOT void unlock() noexcept
  {
    _flag.store(State::Free, std::memory_order_release);
  }

private:
  enum class State : uint8_t
  {
    Free = 0,
    Locked = 1
  };

  std::atomic<State> _flag{State::Free};
};

/***/
class LockGuard
{
public:
  explicit LockGuard(Spinlock& s) : _spinlock(s) { _spinlock.lock(); }
  ~LockGuard() { _spinlock.unlock(); }

  /**
   * Deleted
   */
  LockGuard(LockGuard const&) = delete;
  LockGuard& operator=(LockGuard const&) = delete;

private:
  Spinlock& _spinlock;
};
} // namespace detail

QUILL_END_NAMESPACE