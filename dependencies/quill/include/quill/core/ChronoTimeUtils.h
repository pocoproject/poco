/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"

#include <chrono>
#include <cstdint>

QUILL_BEGIN_NAMESPACE

namespace detail
{
template <typename TClock>
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT std::chrono::nanoseconds get_timestamp() noexcept
{
  return std::chrono::duration_cast<std::chrono::nanoseconds>(TClock::now().time_since_epoch());
}

template <typename TClock>
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT uint64_t get_timestamp_ns() noexcept
{
  return static_cast<uint64_t>(get_timestamp<TClock>().count());
}
} // namespace detail

QUILL_END_NAMESPACE