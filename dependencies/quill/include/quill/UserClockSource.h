/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include <cstdint>

QUILL_BEGIN_NAMESPACE

/**
 * @brief Base class that provides a timestamp for log statements based on a user-provided clock source.
 *
 * This base class can be derived from to pass a user-generated timestamp to a Logger.
 *
 * It is particularly useful for simulations or scenarios where time manipulation is necessary,
 * such as simulating time in the past and displaying past timestamps in logs.
 *
 * @note The derived class must be thread-safe if the Logger object is used across multiple threads.
 * If a Logger is used within a single thread only, thread safety is not a concern.
 */
class UserClockSource
{
public:
  UserClockSource() = default;
  virtual ~UserClockSource() = default;

  UserClockSource(UserClockSource const&) = delete;
  UserClockSource& operator=(UserClockSource const&) = delete;

  /**
   * Returns time since epoch in nanoseconds
   * @return The current timestamp in nanoseconds
   */
  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT virtual uint64_t now() const = 0;
};

QUILL_END_NAMESPACE