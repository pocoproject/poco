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
#include "quill/std/Pair.h"

#include "quill/bundled/fmt/format.h"
#include "quill/bundled/fmt/ranges.h"

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

QUILL_BEGIN_NAMESPACE

template <template <typename...> class UnorderedMapType, typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct Codec<UnorderedMapType<Key, T, Hash, KeyEqual, Allocator>,
             std::enable_if_t<std::disjunction_v<
               std::is_same<UnorderedMapType<Key, T, Hash, KeyEqual, Allocator>, std::unordered_map<Key, T, Hash, KeyEqual, Allocator>>,
               std::is_same<UnorderedMapType<Key, T, Hash, KeyEqual, Allocator>, std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>>>>>
{
  static size_t compute_encoded_size(detail::SizeCacheVector& conditional_arg_size_cache,
                                     UnorderedMapType<Key, T, Hash, KeyEqual, Allocator> const& arg) noexcept
  {
    // We need to store the size of the set in the buffer, so we reserve space for it.
    // We add sizeof(size_t) bytes to accommodate the size information.
    size_t total_size{sizeof(size_t)};

    if constexpr (std::conjunction_v<std::disjunction<std::is_arithmetic<Key>, std::is_enum<Key>>,
                                     std::disjunction<std::is_arithmetic<T>, std::is_enum<T>>>)
    {
      // For built-in types, such as arithmetic or enum types, iteration is unnecessary
      total_size += (sizeof(Key) + sizeof(T)) * arg.size();
    }
    else
    {
      // For other complex types it's essential to determine the exact size of each element.
      // For instance, in the case of a collection of std::string, we need to know the exact size
      // of each string as we will be copying them directly to our queue buffer.
      for (auto const& elem : arg)
      {
        total_size += Codec<Key>::compute_encoded_size(conditional_arg_size_cache, elem.first);
        total_size += Codec<T>::compute_encoded_size(conditional_arg_size_cache, elem.second);
      }
    }

    return total_size;
  }

  template <typename Arg>
  static void encode(std::byte*& buffer, detail::SizeCacheVector const& conditional_arg_size_cache,
                     uint32_t& conditional_arg_size_cache_index, Arg&& arg) noexcept
  {
    Codec<size_t>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index, arg.size());

    // Forward elements based on whether the container was passed as rvalue
    if constexpr (std::is_rvalue_reference_v<Arg&&>)
    {
      for (auto&& elem : arg)
      {
        // Key is const in map, so we copy it; value can be moved
        Codec<Key>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index,
                           elem.first);
        Codec<T>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index,
                         std::move(elem.second));
      }
    }
    else
    {
      for (auto const& elem : arg)
      {
        Codec<Key>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index,
                           elem.first);
        Codec<T>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index, elem.second);
      }
    }
  }

  static auto decode_arg(std::byte*& buffer)
  {
#if defined(_WIN32)
    if constexpr (std::conjunction_v<
                    std::disjunction<std::is_same<UnorderedMapType<Key, T, Hash, KeyEqual, Allocator>, std::unordered_map<Key, T, Hash, KeyEqual, Allocator>>,
                                     std::is_same<UnorderedMapType<Key, T, Hash, KeyEqual, Allocator>, std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>>>,
                    std::disjunction<std::is_same<Key, wchar_t*>, std::is_same<Key, wchar_t const*>, std::is_same<Key, std::wstring>,
                                     std::is_same<Key, std::wstring_view>, std::is_same<T, wchar_t*>, std::is_same<T, wchar_t const*>,
                                     std::is_same<T, std::wstring>, std::is_same<T, std::wstring_view>>>)
    {
      // Read the size of the vector
      size_t const number_of_elements = Codec<size_t>::decode_arg(buffer);

      constexpr bool wide_key_t = std::is_same_v<Key, wchar_t*> || std::is_same_v<Key, wchar_t const*> ||
        std::is_same_v<Key, std::wstring> || std::is_same_v<Key, std::wstring_view>;

      constexpr bool wide_value_t = std::is_same_v<T, wchar_t*> || std::is_same_v<T, wchar_t const*> ||
        std::is_same_v<T, std::wstring> || std::is_same_v<T, std::wstring_view>;

      if constexpr (wide_key_t && !wide_value_t)
      {
        std::vector<std::pair<std::string, T>> encoded_values;
        encoded_values.reserve(number_of_elements);

        for (size_t i = 0; i < number_of_elements; ++i)
        {
          std::pair<std::string, T> elem;
          std::wstring_view v = Codec<Key>::decode_arg(buffer);
          elem.first = detail::utf8_encode(v);
          elem.second = Codec<T>::decode_arg(buffer);
          encoded_values.emplace_back(elem);
        }

        return encoded_values;
      }
      else if constexpr (!wide_key_t && wide_value_t)
      {
        std::vector<std::pair<Key, std::string>> encoded_values;
        encoded_values.reserve(number_of_elements);

        for (size_t i = 0; i < number_of_elements; ++i)
        {
          std::pair<Key, std::string> elem;
          elem.first = Codec<Key>::decode_arg(buffer);
          std::wstring_view v = Codec<T>::decode_arg(buffer);
          elem.second = detail::utf8_encode(v);
          encoded_values.emplace_back(elem);
        }

        return encoded_values;
      }
      else
      {
        std::vector<std::pair<std::string, std::string>> encoded_values;
        encoded_values.reserve(number_of_elements);

        for (size_t i = 0; i < number_of_elements; ++i)
        {
          std::pair<std::string, std::string> elem;
          std::wstring_view v1 = Codec<Key>::decode_arg(buffer);
          elem.first = detail::utf8_encode(v1);
          std::wstring_view v2 = Codec<T>::decode_arg(buffer);
          elem.second = detail::utf8_encode(v2);
          encoded_values.emplace_back(elem);
        }

        return encoded_values;
      }
    }
    else
    {
#endif
      using ReturnKeyType = decltype(Codec<Key>::decode_arg(buffer));
      using ReturnValueType = decltype(Codec<T>::decode_arg(buffer));
      using ReboundAllocator =
        typename std::allocator_traits<Allocator>::template rebind_alloc<std::pair<ReturnKeyType const, ReturnValueType>>;
      UnorderedMapType<ReturnKeyType, ReturnValueType, Hash, KeyEqual, ReboundAllocator> arg;

      // Read the size of the set
      size_t const number_of_elements = Codec<size_t>::decode_arg(buffer);
      arg.reserve(number_of_elements);

      for (size_t i = 0; i < number_of_elements; ++i)
      {
        auto key = Codec<Key>::decode_arg(buffer);
        auto value = Codec<T>::decode_arg(buffer);

        if constexpr (std::is_move_constructible_v<ReturnKeyType> && std::is_move_constructible_v<ReturnValueType>)
        {
          arg.insert({std::move(key), std::move(value)});
        }
        else if constexpr (std::is_move_constructible_v<ReturnKeyType>)
        {
          arg.insert({std::move(key), value});
        }
        else if constexpr (std::is_move_constructible_v<ReturnValueType>)
        {
          arg.insert({key, std::move(value)});
        }
        else
        {
          arg.insert({key, value});
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