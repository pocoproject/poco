/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>

#include "quill/core/Attributes.h"
#include "quill/core/Codec.h"
#include "quill/core/DynamicFormatArgStore.h"

QUILL_BEGIN_NAMESPACE

namespace utility
{
/**
 * StringRef is used to specify that a string argument should be passed by reference instead of by
 * value, ensuring that no copy of the string is made.
 *
 * Note that by default, all strings, including C strings and std::string_view, are copied before
 * being passed to the backend. To pass strings by reference, they must be wrapped in a StringRef.
 *
 * Use this with caution, as the backend will parse the string asynchronously. The wrapped string
 * must have a valid lifetime and should not be modified.
 */
class StringRef
{
public:
  explicit StringRef(std::string const& str) : _str_view(str) {};
  explicit StringRef(std::string_view str) : _str_view(str) {};
  explicit StringRef(char const* str) : _str_view(str, detail::safe_strnlen(str)) {};
  StringRef(char const* str, size_t size) : _str_view(str, size) {};

  QUILL_NODISCARD std::string_view const& get_string_view() const noexcept { return _str_view; }

private:
  std::string_view _str_view;
};
} // namespace utility

/***/
template <>
struct Codec<utility::StringRef>
{
  static size_t compute_encoded_size(detail::SizeCacheVector&, utility::StringRef const&) noexcept
  {
    return sizeof(size_t) + sizeof(uintptr_t);
  }

  static void encode(std::byte*& buffer, detail::SizeCacheVector const&, uint32_t&,
                     utility::StringRef const& no_copy) noexcept
  {
    char const* data = no_copy.get_string_view().data();
    std::memcpy(buffer, &data, sizeof(uintptr_t));
    buffer += sizeof(uintptr_t);

    size_t const size = no_copy.get_string_view().size();
    std::memcpy(buffer, &size, sizeof(size_t));
    buffer += sizeof(size_t);
  }

  static std::string_view decode_arg(std::byte*& buffer)
  {
    char const* data;
    std::memcpy(&data, buffer, sizeof(uintptr_t));
    buffer += sizeof(uintptr_t);

    size_t size;
    std::memcpy(&size, buffer, sizeof(size_t));
    buffer += sizeof(size_t);

    return std::string_view{data, size};
  }

  static void decode_and_store_arg(std::byte*& buffer, DynamicFormatArgStore* args_store)
  {
    args_store->push_back(decode_arg(buffer));
  }
};

QUILL_END_NAMESPACE
