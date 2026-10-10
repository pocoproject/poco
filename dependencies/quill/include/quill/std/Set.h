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
#include <set>
#include <type_traits>
#include <utility>
#include <vector>

QUILL_BEGIN_NAMESPACE

template <template <typename...> class SetType, typename Key, typename Compare, typename Allocator>
struct Codec<SetType<Key, Compare, Allocator>,
             std::enable_if_t<std::disjunction_v<std::is_same<SetType<Key, Compare, Allocator>, std::set<Key, Compare, Allocator>>,
                                                 std::is_same<SetType<Key, Compare, Allocator>, std::multiset<Key, Compare, Allocator>>>>>
{
  static size_t compute_encoded_size(detail::SizeCacheVector& conditional_arg_size_cache,
                                     SetType<Key, Compare, Allocator> const& arg) noexcept
  {
    // We need to store the size of the set in the buffer, so we reserve space for it.
    // We add sizeof(size_t) bytes to accommodate the size information.
    size_t total_size{sizeof(size_t)};

    if constexpr (std::is_arithmetic_v<Key>)
    {
      // Built-in arithmetic types don't require iteration.
      // Note: Enums are excluded as they may have custom Codecs (e.g., DirectFormatCodec)
      total_size += sizeof(Key) * arg.size();
    }
    else
    {
      // For other complex types it's essential to determine the exact size of each element.
      // For instance, in the case of a collection of std::string, we need to know the exact size
      // of each string as we will be copying them directly to our queue buffer.
      for (auto const& elem : arg)
      {
        total_size += Codec<Key>::compute_encoded_size(conditional_arg_size_cache, elem);
      }
    }

    return total_size;
  }

  template <typename Arg>
  static void encode(std::byte*& buffer, detail::SizeCacheVector const& conditional_arg_size_cache,
                     uint32_t& conditional_arg_size_cache_index, Arg&& arg) noexcept
  {
    Codec<size_t>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index, arg.size());

    if constexpr (std::is_rvalue_reference_v<Arg&&>)
    {
      for (auto&& elem : arg)
      {
        Codec<Key>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index,
                           std::move(elem));
      }
    }
    else
    {
      for (auto const& elem : arg)
      {
        Codec<Key>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index, elem);
      }
    }
  }

  static auto decode_arg(std::byte*& buffer)
  {
#if defined(_WIN32)
    if constexpr (std::conjunction_v<
                    std::disjunction<std::is_same<SetType<Key, Compare, Allocator>, std::set<Key, Compare, Allocator>>,
                                     std::is_same<SetType<Key, Compare, Allocator>, std::multiset<Key, Compare, Allocator>>>,
                    std::disjunction<std::is_same<Key, wchar_t*>, std::is_same<Key, wchar_t const*>,
                                     std::is_same<Key, std::wstring>, std::is_same<Key, std::wstring_view>>>)
    {
      // Read the size of the vector
      size_t const number_of_elements = Codec<size_t>::decode_arg(buffer);

      std::vector<std::string> encoded_values;
      encoded_values.reserve(number_of_elements);

      for (size_t i = 0; i < number_of_elements; ++i)
      {
        std::wstring_view v = Codec<Key>::decode_arg(buffer);
        encoded_values.emplace_back(detail::utf8_encode(v));
      }

      return encoded_values;
    }
    else
    {
#endif
      using ReturnType = decltype(Codec<Key>::decode_arg(buffer));
      using ReboundAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<ReturnType>;
      using ReboundCompare =
        typename std::conditional<std::is_same<Compare, std::less<Key>>::value, std::less<ReturnType>, Compare>::type;

      SetType<ReturnType, ReboundCompare, ReboundAllocator> arg;

      size_t const number_of_elements = Codec<size_t>::decode_arg(buffer);

      for (size_t i = 0; i < number_of_elements; ++i)
      {
        auto elem = Codec<Key>::decode_arg(buffer);
        if constexpr (std::is_move_constructible_v<ReturnType>)
        {
          arg.emplace(std::move(elem));
        }
        else
        {
          arg.emplace(elem);
        }
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