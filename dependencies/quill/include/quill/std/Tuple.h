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
#include <tuple>
#include <type_traits>
#include <utility>

QUILL_BEGIN_NAMESPACE

template <typename... Types>
struct Codec<std::tuple<Types...>>
{
  static size_t compute_encoded_size(detail::SizeCacheVector& conditional_arg_size_cache,
                                     std::tuple<Types...> const& arg) noexcept
  {
    size_t total_size{0};

    std::apply(
      [&total_size, &conditional_arg_size_cache](auto const&... elems)
      {
        ((total_size += Codec<std::decay_t<decltype(elems)>>::compute_encoded_size(conditional_arg_size_cache, elems)),
         ...);
      },
      arg);

    return total_size;
  }

  template <typename Arg>
  static void encode(std::byte*& buffer, detail::SizeCacheVector const& conditional_arg_size_cache,
                     uint32_t& conditional_arg_size_cache_index, Arg&& arg) noexcept
  {
    if constexpr (std::is_rvalue_reference_v<Arg&&>)
    {
      std::apply(
        [&conditional_arg_size_cache, &conditional_arg_size_cache_index, &buffer](auto&&... elems)
        {
          ((Codec<std::decay_t<decltype(elems)>>::encode(
             buffer, conditional_arg_size_cache, conditional_arg_size_cache_index, std::move(elems))),
           ...);
        },
        std::move(arg));
    }
    else
    {
      std::apply(
        [&conditional_arg_size_cache, &conditional_arg_size_cache_index, &buffer](auto const&... elems)
        {
          ((Codec<std::decay_t<decltype(elems)>>::encode(buffer, conditional_arg_size_cache,
                                                         conditional_arg_size_cache_index, elems)),
           ...);
        },
        arg);
    }
  }

  static auto decode_arg(std::byte*& buffer)
  {
    // Decode elements in order and construct tuple
    // Note: Function argument evaluation order is unspecified, so we must ensure ordered evaluation
    std::tuple<decltype(Codec<Types>::decode_arg(buffer))...> arg;

    size_t index = 0;
    std::apply(
      [&buffer, &index](auto&... elems)
      {
        // Use fold expression with comma operator to ensure left-to-right evaluation
        (...,
         (
           [&buffer, &index, &elems]()
           {
             using ElemType = std::decay_t<decltype(elems)>;
             auto decoded = Codec<ElemType>::decode_arg(buffer);

             if constexpr (std::is_move_assignable_v<ElemType> && std::is_move_constructible_v<decltype(decoded)>)
             {
               elems = std::move(decoded);
             }
             else
             {
               elems = decoded;
             }
             ++index;
           }()));
      },
      arg);

    // Use static_cast to explicitly select copy or move based on move constructibility
    if constexpr (std::conjunction_v<std::is_move_constructible<decltype(Codec<Types>::decode_arg(buffer))>...>)
    {
      return arg; // Eligible for move
    }
    else
    {
      // Force copy construction for tuples with copy-only elements
      using TupleType = std::tuple<decltype(Codec<Types>::decode_arg(buffer))...>;
      return static_cast<TupleType const&>(arg);
    }
  }

  static void decode_and_store_arg(std::byte*& buffer, DynamicFormatArgStore* args_store)
  {
    // Check if all tuple elements are move constructible
    if constexpr (std::conjunction_v<std::is_move_constructible<decltype(Codec<Types>::decode_arg(buffer))>...>)
    {
      // All elements are move constructible, decode and move
      auto arg = decode_arg(buffer);
      args_store->push_back(std::move(arg));
    }
    else
    {
      // Some elements are copy-only, decode and copy
      // Use const auto& to ensure copy construction is used
      auto const arg = decode_arg(buffer);
      args_store->push_back(arg);
    }
  }
};

QUILL_END_NAMESPACE