/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "quill/core/Attributes.h"

QUILL_BEGIN_NAMESPACE

namespace utility
{
/**
 * @brief Formats the given buffer to hexadecimal representation.
 *
 * This function converts the contents of the input buffer to a hexadecimal string.
 *
 * @param buffer Pointer to the input buffer.
 * @param size Size of the input buffer.
 * @param uppercase If true, use uppercase hex characters (A-F); if false, use lowercase (a-f).
 * @param space_delim If true, adds space between each byte in the output; if false, outputs continuous hex string.
 * @return A string containing the hexadecimal representation of the given buffer.
 */
template <typename T>
QUILL_NODISCARD std::string to_hex(T const* buffer, size_t size, bool uppercase = true, bool space_delim = true)
{
  static_assert(sizeof(T) == 1, "to_hex only accepts byte-sized element types");

  static constexpr char hex_table_lower[513] =
    "000102030405060708090a0b0c0d0e0f"
    "101112131415161718191a1b1c1d1e1f"
    "202122232425262728292a2b2c2d2e2f"
    "303132333435363738393a3b3c3d3e3f"
    "404142434445464748494a4b4c4d4e4f"
    "505152535455565758595a5b5c5d5e5f"
    "606162636465666768696a6b6c6d6e6f"
    "707172737475767778797a7b7c7c7e7f"
    "808182838485868788898a8b8c8d8e8f"
    "909192939495969798999a9b9c9d9e9f"
    "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
    "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
    "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
    "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
    "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
    "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";

  static constexpr char hex_table_upper[513] =
    "000102030405060708090A0B0C0D0E0F"
    "101112131415161718191A1B1C1D1E1F"
    "202122232425262728292A2B2C2D2E2F"
    "303132333435363738393A3B3C3D3E3F"
    "404142434445464748494A4B4C4D4E4F"
    "505152535455565758595A5B5C5D5E5F"
    "606162636465666768696A6B6C6D6E6F"
    "707172737475767778797A7B7C7D7E7F"
    "808182838485868788898A8B8C8D8E8F"
    "909192939495969798999A9B9C9D9E9F"
    "A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
    "B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
    "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
    "D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
    "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
    "F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

  if (QUILL_UNLIKELY(!buffer || size == 0))
  {
    return std::string{};
  }

  char const* hex_table = uppercase ? hex_table_upper : hex_table_lower;
  size_t out_size = space_delim ? size * 3 - 1 : size * 2;
  std::string hex_string(out_size, '\0');
  char* dest_ptr = &hex_string[0];

  size_t i = 0;
  size_t const unroll_count = 4;
  size_t aligned_size = size & ~(unroll_count - 1); // round down to multiple of 4

  while (i < aligned_size)
  {
    for (size_t j = 0; j < unroll_count; ++j)
    {
      auto ubyte = static_cast<uint8_t>(buffer[i + j]);
      char const* hex_p = &hex_table[ubyte * 2];
      dest_ptr[0] = hex_p[0];
      dest_ptr[1] = hex_p[1];
      dest_ptr += 2;
      if (space_delim && (i + j + 1 != size))
      {
        *dest_ptr++ = ' ';
      }
    }
    i += unroll_count;
  }

  // Handle remaining bytes
  for (; i < size; ++i)
  {
    auto ubyte = static_cast<uint8_t>(buffer[i]);
    char const* hex_p = &hex_table[ubyte * 2];
    dest_ptr[0] = hex_p[0];
    dest_ptr[1] = hex_p[1];
    dest_ptr += 2;
    if (space_delim && (i + 1 != size))
    {
      *dest_ptr++ = ' ';
    }
  }

  return hex_string;
}
} // namespace utility

QUILL_END_NAMESPACE