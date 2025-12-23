/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#if defined(_WIN32)
  #include "quill/core/Attributes.h"
  #include "quill/core/Codec.h"
  #include "quill/core/DynamicFormatArgStore.h"
  #include "quill/core/InlinedVector.h"

  #include "quill/bundled/fmt/format.h"

  #include <cstddef>
  #include <cstdint>
  #include <limits>
  #include <string>
  #include <string_view>
  #include <type_traits>

QUILL_BEGIN_NAMESPACE

/** Specialization for arrays of arithmetic types and enums **/
template <typename T>
struct Codec<T,
             std::enable_if_t<std::disjunction_v<std::is_same<T, wchar_t*>, std::is_same<T, wchar_t const*>,
                                                 std::is_same<T, std::wstring>, std::is_same<T, std::wstring_view>>>>
{
  static size_t compute_encoded_size(detail::SizeCacheVector& conditional_arg_size_cache, T const& arg) noexcept
  {
    size_t len;

    if constexpr (std::disjunction_v<std::is_same<T, wchar_t*>, std::is_same<T, wchar_t const*>>)
    {
      len = wcslen(arg);
    }
    else
    {
      len = arg.size();
    }

    QUILL_ASSERT(
      len <= std::numeric_limits<uint32_t>::max(),
      "Wide string length exceeds uint32_t max in Codec<std::wstring>::compute_encoded_size()");
    conditional_arg_size_cache.push_back(static_cast<uint32_t>(len));

    // also include the size of the string in the buffer as a separate variable
    // we can retrieve it when we decode. We do not store the null terminator in the buffer
    return sizeof(uint32_t) + (static_cast<uint32_t>(len) * sizeof(wchar_t));
  }

  static void encode(std::byte*& buffer, detail::SizeCacheVector const& conditional_arg_size_cache,
                     uint32_t& conditional_arg_size_cache_index, T const& arg) noexcept
  {
    // The wide string size in bytes
    uint32_t const len = conditional_arg_size_cache[conditional_arg_size_cache_index++];
    std::memcpy(buffer, &len, sizeof(len));
    buffer += sizeof(len);

    if (len != 0)
    {
      // copy the string, no need to zero terminate it as we got the length and e.g a wstring_view
      // might not always be zero terminated
      size_t const size_in_bytes = len * sizeof(wchar_t);

      if constexpr (std::disjunction_v<std::is_same<T, wchar_t*>, std::is_same<T, wchar_t const*>>)
      {
        std::memcpy(buffer, arg, size_in_bytes);
      }
      else
      {
        std::memcpy(buffer, arg.data(), size_in_bytes);
      }

      buffer += size_in_bytes;
    }
  }

  static std::wstring_view decode_arg(std::byte*& buffer)
  {
    uint32_t len;
    std::memcpy(&len, buffer, sizeof(len));
    buffer += sizeof(len);
    auto arg = std::wstring_view{reinterpret_cast<wchar_t const*>(buffer), len};
    buffer += len * sizeof(wchar_t);
    return arg;
  }

  static void decode_and_store_arg(std::byte*& buffer, DynamicFormatArgStore* args_store)
  {
    std::wstring_view const arg = decode_arg(buffer);
    args_store->push_back(detail::utf8_encode(reinterpret_cast<std::byte const*>(arg.data()), arg.size()));
  }
};

QUILL_END_NAMESPACE

#endif