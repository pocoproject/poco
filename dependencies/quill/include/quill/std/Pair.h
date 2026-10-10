/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Codec.h"
#include "quill/core/DynamicFormatArgStore.h"
#include "quill/core/InlinedVector.h"

#include "quill/bundled/fmt/format.h"
#include "quill/bundled/fmt/ranges.h"

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

QUILL_BEGIN_NAMESPACE

template <typename T1, typename T2>
struct Codec<std::pair<T1, T2>>
{
  static size_t compute_encoded_size(detail::SizeCacheVector& conditional_arg_size_cache,
                                     std::pair<T1, T2> const& arg) noexcept
  {
    // Explicitly separate the calls to ensure the order of evaluation is maintained,
    // as the compiler may evaluate the expressions in a different order, leading to side effects.
    size_t total_size = Codec<T1>::compute_encoded_size(conditional_arg_size_cache, arg.first);
    total_size += Codec<T2>::compute_encoded_size(conditional_arg_size_cache, arg.second);
    return total_size;
  }

  template <typename Arg>
  static void encode(std::byte*& buffer, detail::SizeCacheVector const& conditional_arg_size_cache,
                     uint32_t& conditional_arg_size_cache_index, Arg&& arg) noexcept
  {
    if constexpr (std::is_rvalue_reference_v<Arg&&>)
    {
      Codec<T1>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index,
                        std::move(arg.first));
      Codec<T2>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index,
                        std::move(arg.second));
    }
    else
    {
      Codec<T1>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index, arg.first);
      Codec<T2>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index, arg.second);
    }
  }

  static auto decode_arg(std::byte*& buffer)
  {
#if defined(_WIN32)
    if constexpr (std::disjunction_v<std::is_same<T1, wchar_t*>, std::is_same<T1, wchar_t const*>,
                                     std::is_same<T1, std::wstring>, std::is_same<T1, std::wstring_view>,
                                     std::is_same<T2, wchar_t*>, std::is_same<T2, wchar_t const*>,
                                     std::is_same<T2, std::wstring>, std::is_same<T2, std::wstring_view>>)
    {
      constexpr bool wide_t1 = std::is_same_v<T1, wchar_t*> || std::is_same_v<T1, wchar_t const*> ||
        std::is_same_v<T1, std::wstring> || std::is_same_v<T1, std::wstring_view>;

      constexpr bool wide_t2 = std::is_same_v<T2, wchar_t*> || std::is_same_v<T2, wchar_t const*> ||
        std::is_same_v<T2, std::wstring> || std::is_same_v<T2, std::wstring_view>;

      if constexpr (wide_t1 && !wide_t2)
      {
        std::pair<std::string, T2> arg;

        std::wstring_view v = Codec<T1>::decode_arg(buffer);
        arg.first = detail::utf8_encode(v);

        arg.second = Codec<T2>::decode_arg(buffer);

        return arg;
      }
      else if constexpr (!wide_t1 && wide_t2)
      {
        std::pair<T1, std::string> arg;

        arg.first = Codec<T1>::decode_arg(buffer);

        std::wstring_view v = Codec<T2>::decode_arg(buffer);
        arg.second = detail::utf8_encode(v);

        return arg;
      }
      else
      {
        std::pair<std::string, std::string> arg;
        std::wstring_view v1 = Codec<T1>::decode_arg(buffer);
        arg.first = detail::utf8_encode(v1);

        std::wstring_view v2 = Codec<T2>::decode_arg(buffer);
        arg.second = detail::utf8_encode(v2);

        return arg;
      }
    }
    else
    {
#endif
      using ReturnType1 = decltype(Codec<T1>::decode_arg(buffer));
      using ReturnType2 = decltype(Codec<T2>::decode_arg(buffer));
      std::pair<ReturnType1, ReturnType2> arg;

      auto elem1 = Codec<T1>::decode_arg(buffer);
      if constexpr (std::is_move_constructible_v<ReturnType1>)
      {
        arg.first = std::move(elem1);
      }
      else
      {
        arg.first = elem1;
      }

      auto elem2 = Codec<T2>::decode_arg(buffer);
      if constexpr (std::is_move_constructible_v<ReturnType2>)
      {
        arg.second = std::move(elem2);
      }
      else
      {
        arg.second = elem2;
      }

      return arg;

#if defined(_WIN32)
    }
#endif
  }

  static void decode_and_store_arg(std::byte*& buffer, DynamicFormatArgStore* args_store)
  {
    args_store->push_back(decode_arg(buffer));
  }
};

QUILL_END_NAMESPACE