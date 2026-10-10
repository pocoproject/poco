/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Codec.h"
#include "quill/core/DynamicFormatArgStore.h"
#include "quill/core/Filesystem.h"
#include "quill/core/InlinedVector.h"

#include "quill/bundled/fmt/format.h"
#include "quill/bundled/fmt/std.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>

#if defined(_WIN32)
  #include "quill/std/WideString.h"
#endif

QUILL_BEGIN_NAMESPACE

template <>
struct Codec<fs::path>
{
  static size_t compute_encoded_size(detail::SizeCacheVector& conditional_arg_size_cache, fs::path const& arg) noexcept
  {
    if constexpr (std::is_same_v<fs::path::string_type, std::string>)
    {
      return Codec<std::string>::compute_encoded_size(conditional_arg_size_cache, arg.string());
    }
#if defined(_WIN32)
    else if constexpr (std::is_same_v<fs::path::string_type, std::wstring>)
    {
      return Codec<std::wstring>::compute_encoded_size(conditional_arg_size_cache, arg.wstring());
    }
#endif
  }

  static void encode(std::byte*& buffer, detail::SizeCacheVector const& conditional_arg_size_cache,
                     uint32_t& conditional_arg_size_cache_index, fs::path const& arg) noexcept
  {
    if constexpr (std::is_same_v<fs::path::string_type, std::string>)
    {
      Codec<std::string>::encode(buffer, conditional_arg_size_cache,
                                 conditional_arg_size_cache_index, arg.string());
    }
#if defined(_WIN32)
    else if constexpr (std::is_same_v<fs::path::string_type, std::wstring>)
    {
      Codec<std::wstring>::encode(buffer, conditional_arg_size_cache,
                                  conditional_arg_size_cache_index, arg.wstring());
    }
#endif
  }

  static fs::path decode_arg(std::byte*& buffer)
  {
    if constexpr (std::is_same_v<fs::path::string_type, std::string>)
    {
      return fs::path{Codec<std::string_view>::decode_arg(buffer)};
    }
#if defined(_WIN32)
    else if constexpr (std::is_same_v<fs::path::string_type, std::wstring>)
    {
      return fs::path{Codec<std::wstring_view>::decode_arg(buffer)};
    }
#endif
  }

  static void decode_and_store_arg(std::byte*& buffer, DynamicFormatArgStore* args_store)
  {
    args_store->push_back(decode_arg(buffer));
  }
};

QUILL_END_NAMESPACE