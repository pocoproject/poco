/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/bundled/fmt/base.h"
#include "quill/core/Attributes.h"
#include "quill/core/Codec.h"
#include "quill/core/DynamicFormatArgStore.h"
#include "quill/core/InlinedVector.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>

QUILL_BEGIN_NAMESPACE

/**
 * @brief Provides codec functionality for serializing complex user-defined types into strings.
 *
 * This codec is particularly useful for logging user-defined types, it converts an object into a
 * string representation using the `fmt::formatter`on the hot path.
 *
 * When using this codec, a `fmt::formatter` specialization must exist for the user-defined type.
 *
 * It eliminates the need to explicitly write more verbose code, such as:
 *
 * ```cpp
 * LOG_INFO(l, "{}", fmtquill::format("{}", obj));
 * ```
 *
 * Instead, you can directly write:
 *
 * ```cpp
 * LOG_INFO(logger, "{}", obj);
 * ```
 *
 * \code{.cpp}
 * class User
 * {
 * public:
 *   User(std::string name, std::string surname, uint32_t age)
 *     : name(std::move(name)), surname(std::move(surname)), age(age)
 *   {
 *     favorite_colors.push_back("red");
 *     favorite_colors.push_back("blue");
 *     favorite_colors.push_back("green");
 *   };
 *
 *   std::string name;
 *   std::string surname;
 *   uint32_t age;
 *   std::vector<std::string> favorite_colors;
 * };
 *
 * template <>
 * struct fmtquill::formatter<User>
 * {
 *   constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
 *
 *   auto format(::User const& user, format_context& ctx) const
 *   {
 *     return fmtquill::format_to(ctx.out(), "Name: {}, Surname: {}, Age: {}, Favorite Colors: {}",
 *                                user.name, user.surname, user.age, user.favorite_colors);
 *   }
 * };
 *
 * template <>
 * struct quill::Codec<User> : quill::DirectFormatCodec<User>
 * {
 * };
 *
 * int main()
 * {
 *   // ... init code
 *   User user_1{"Super", "User", 1};
 *   LOG_INFO(logger, "User is [{}]", user_1);
 * }
 * \endcode
 */

template <typename T>
struct DirectFormatCodec
{
  static size_t compute_encoded_size(quill::detail::SizeCacheVector& conditional_arg_size_cache, T const& arg)
  {
    // The computed size includes the size of the length field
    return sizeof(uint32_t) +
      conditional_arg_size_cache.push_back(static_cast<uint32_t>(fmtquill::formatted_size("{}", arg)));
  }

  static void encode(std::byte*& buffer, quill::detail::SizeCacheVector const& conditional_arg_size_cache,
                     uint32_t& conditional_arg_size_cache_index, T const& arg)
  {
    uint32_t const len = conditional_arg_size_cache[conditional_arg_size_cache_index++];
    std::memcpy(buffer, &len, sizeof(len));
    buffer += sizeof(len);
    fmtquill::format_to_n(reinterpret_cast<char*>(buffer), len, "{}", arg);
    buffer += len;
  }

  static std::string_view decode_arg(std::byte*& buffer)
  {
    return quill::Codec<std::string>::decode_arg(buffer);
  }

  static void decode_and_store_arg(std::byte*& buffer, quill::DynamicFormatArgStore* args_store)
  {
    args_store->push_back(decode_arg(buffer));
  }
};

QUILL_END_NAMESPACE