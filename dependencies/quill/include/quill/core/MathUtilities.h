/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"

#include <cstdint>
#include <limits>

QUILL_BEGIN_NAMESPACE

namespace detail
{
/**
 * Check if a number is a power of 2
 * @param number the number to check against
 * @return true if the number is power of 2
 *         false otherwise
 */
QUILL_NODISCARD constexpr bool is_power_of_two(uint64_t number) noexcept
{
  return (number != 0) && ((number & (number - 1)) == 0);
}

/**
 * Helper function to calculate the maximum power of two for type T
 * @return maximum power of two for type T
 */
template <typename T>
QUILL_NODISCARD constexpr T max_power_of_two() noexcept
{
  return (std::numeric_limits<T>::max() >> 1) + 1;
}

/**
 * Round up to the next power of 2
 * @param n input
 * @return the next power of 2
 */
template <typename T>
QUILL_NODISCARD T next_power_of_two(T n) noexcept
{
  constexpr T max_power_of_2 = max_power_of_two<T>();

  if (n >= max_power_of_2)
  {
    return max_power_of_2;
  }

  if (is_power_of_two(static_cast<uint64_t>(n)))
  {
    return n;
  }

  T result = 1;
  while (result < n)
  {
    result <<= 1;
  }

  QUILL_ASSERT(is_power_of_two(static_cast<uint64_t>(result)),
               "result is not a power of 2 in next_power_of_two()");

  return result;
}
} // namespace detail

QUILL_END_NAMESPACE