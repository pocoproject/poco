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

#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

#if defined(_WIN32)
  #include <string>
#endif

QUILL_BEGIN_NAMESPACE

/** Specialization for arrays of arithmetic types and enums, char arrays are handled in Codec.h **/
template <typename T, std::size_t N>
struct Codec<T[N], std::enable_if_t<std::negation_v<std::is_same<T, char>>>>
{
  static size_t compute_encoded_size(detail::SizeCacheVector& conditional_arg_size_cache, T const (&arg)[N]) noexcept
  {
    if constexpr (std::is_arithmetic_v<T>)
    {
      // Built-in arithmetic types don't require iteration.
      // Note: Enums are excluded as they may have custom Codecs (e.g., DirectFormatCodec)
      return sizeof(T) * N;
    }
    else
    {
      // For other complex types it's essential to determine the exact size of each element.
      // For instance, in the case of a collection of std::string, we need to know the exact size
      // of each string as we will be copying them directly to our queue buffer.
      size_t total_size{0};

      for (auto const& elem : arg)
      {
        total_size += Codec<T>::compute_encoded_size(conditional_arg_size_cache, elem);
      }

      return total_size;
    }
  }

  template <typename Arg>
  static void encode(std::byte*& buffer, detail::SizeCacheVector const& conditional_arg_size_cache,
                     uint32_t& conditional_arg_size_cache_index, Arg&& arg) noexcept
  {
    if constexpr (std::is_arithmetic_v<T>)
    {
      // Built-in arithmetic types don't require iteration.
      // Note: Enums are excluded as they may have custom Codecs (e.g., DirectFormatCodec)
      std::memcpy(buffer, &arg, sizeof(T) * N);
      buffer += sizeof(T) * N;
    }
    else
    {
      if constexpr (std::is_rvalue_reference_v<Arg&&>)
      {
        for (size_t i = 0; i < N; ++i)
        {
          Codec<T>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index,
                           std::move(arg[i]));
        }
      }
      else
      {
        for (size_t i = 0; i < N; ++i)
        {
          Codec<T>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index, arg[i]);
        }
      }
    }
  }

  static auto decode_arg(std::byte*& buffer)
  {
    using ReturnType =
      std::conditional_t<std::is_same_v<T, std::string>, std::string, decltype(Codec<T>::decode_arg(buffer))>;

    std::array<ReturnType, N> arg;

    for (size_t i = 0; i < N; ++i)
    {
      auto elem = Codec<T>::decode_arg(buffer);
      if constexpr (std::is_move_constructible_v<ReturnType>)
      {
        arg[i] = std::move(elem);
      }
      else
      {
        arg[i] = elem;
      }
    }

    return arg;
  }

  static void decode_and_store_arg(std::byte*& buffer, DynamicFormatArgStore* args_store)
  {
    args_store->push_back(decode_arg(buffer));
  }
};

/** Specialization for std::array **/
template <typename T, size_t N>
struct Codec<std::array<T, N>>
{
  static size_t compute_encoded_size(detail::SizeCacheVector& conditional_arg_size_cache,
                                     std::array<T, N> const& arg) noexcept
  {
    if constexpr (std::is_arithmetic_v<T>)
    {
      // Built-in arithmetic types don't require iteration.
      // Note: Enums are excluded as they may have custom Codecs (e.g., DirectFormatCodec)
      return sizeof(T) * N;
    }
    else
    {
      // For other complex types it's essential to determine the exact size of each element.
      // For instance, in the case of a collection of std::string, we need to know the exact size
      // of each string as we will be copying them directly to our queue buffer.
      size_t total_size{0};

      for (auto const& elem : arg)
      {
        total_size += Codec<T>::compute_encoded_size(conditional_arg_size_cache, elem);
      }

      return total_size;
    }
  }

  template <typename Arg>
  static void encode(std::byte*& buffer, detail::SizeCacheVector const& conditional_arg_size_cache,
                     uint32_t& conditional_arg_size_cache_index, Arg&& arg) noexcept
  {
    if constexpr (std::is_arithmetic_v<T>)
    {
      // Built-in arithmetic types don't require iteration.
      // Note: Enums are excluded as they may have custom Codecs (e.g., DirectFormatCodec)
      std::memcpy(buffer, arg.data(), sizeof(T) * N);
      buffer += sizeof(T) * N;
    }
    else
    {
      if constexpr (std::is_rvalue_reference_v<Arg&&>)
      {
        for (auto&& elem : arg)
        {
          Codec<T>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index,
                           std::move(elem));
        }
      }
      else
      {
        for (auto const& elem : arg)
        {
          Codec<T>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index, elem);
        }
      }
    }
  }

  static auto decode_arg(std::byte*& buffer)
  {
#if defined(_WIN32)
    // Wide string support
    if constexpr (std::disjunction_v<std::is_same<T, wchar_t*>, std::is_same<T, wchar_t const*>,
                                     std::is_same<T, std::wstring>, std::is_same<T, std::wstring_view>>)
    {
      std::vector<std::string> arg;
      arg.reserve(N);

      for (size_t i = 0; i < N; ++i)
      {
        std::wstring_view v = Codec<T>::decode_arg(buffer);
        arg.emplace_back(detail::utf8_encode(v));
      }

      return arg;
    }
    else
    {
#endif
      using ReturnType =
        std::conditional_t<std::is_same_v<T, std::string>, std::string, decltype(Codec<T>::decode_arg(buffer))>;

      std::array<ReturnType, N> arg;

      for (size_t i = 0; i < N; ++i)
      {
        auto elem = Codec<T>::decode_arg(buffer);
        if constexpr (std::is_move_constructible_v<ReturnType>)
        {
          arg[i] = std::move(elem);
        }
        else
        {
          arg[i] = elem;
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