/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/bundled/fmt/base.h"

#include "quill/core/Attributes.h"
#include "quill/core/DynamicFormatArgStore.h"
#include "quill/core/InlinedVector.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>

QUILL_BEGIN_NAMESPACE

namespace detail
{

#if defined(_WIN32)
  #if defined(__MINGW32__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wredundant-decls"
  #endif

/** We forward declare these to avoid including Utf8Conv.h **/
QUILL_NODISCARD QUILL_EXPORT QUILL_ATTRIBUTE_USED extern std::string utf8_encode(std::wstring_view str);
QUILL_NODISCARD QUILL_EXPORT QUILL_ATTRIBUTE_USED extern std::string utf8_encode(std::byte const* data,
                                                                                 size_t wide_str_len);

  #if defined(__MINGW32__)
    #pragma GCC diagnostic pop
  #endif
#endif

/**
 * C++14 implementation of C++20's remove_cvref
 */
template <class T>
struct remove_cvref
{
  typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <class>
constexpr bool always_false_v = false;

template <typename Arg>
void codec_not_found_for_type()
{
  static_assert(
    always_false_v<Arg>,
    "\n"
    "+------------------------------------------------------------------------------+\n"
    "|                       Missing Codec for Type 'Arg'                           |\n"
    "+------------------------------------------------------------------------------+\n"
    "\n"
    "Error: A codec for the specified type 'Arg' is not available.\n"
    "\n"
    "Possible solutions:\n"
    "1. If Arg is an STL type:\n"
    "   - Ensure you have included the necessary headers for the specific STL type you are using "
    "from the quill/std folder.\n"
    "\n"
    "2. If Arg is a user-defined type:\n"
    "   - Use either 'DeferredFormatCodec' or 'DirectFormatCodec'.\n"
    "   - Define a custom Codec for your type.\n"
    "   - Consider converting the value to a string before logging.\n"
    "\n"
    "Note: The specific type of 'Arg' can be found in the compiler error message.\n"
    "      Look for the instantiation of 'codec_not_found_for_type<Arg>' in the error output.\n"
    "      The compiler should indicate what type 'Arg' represents in this instance.\n"
    "\n"
    "For more information see https://quillcpp.readthedocs.io/en/latest/cheat_sheet.html\n");
}

/***/
QUILL_NODISCARD inline size_t safe_strnlen(char const* str, size_t maxlen) noexcept
{
  if (!str)
  {
    return 0;
  }

#if defined(__GNUC__) && !defined(__clang__)
  // Suppress false positive in GCC - memchr safely stops at null terminator
  #pragma GCC diagnostic push
  #if __GNUC__ >= 13
    #pragma GCC diagnostic ignored "-Wstringop-overread"
  #endif

  // Suppress during LTO analysis
  asm volatile("" : "+r"(maxlen) : : "memory");
#endif

  auto end = static_cast<char const*>(std::memchr(str, '\0', maxlen));

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC diagnostic pop
#endif

  return end ? static_cast<size_t>(end - str) : maxlen;
}

/***/
QUILL_NODISCARD inline size_t safe_strnlen(char const* str) noexcept
{
#if defined(__i386__) || defined(__arm__)
  // On i386, armel and armhf std::memchr "max number of bytes to examine" set to maximum size of
  // unsigned int which does not compile
  // Currently Debian package is using architecture `any` which includes them
  static constexpr int32_t max_len = std::numeric_limits<int32_t>::max();
#else
  static constexpr uint32_t max_len = std::numeric_limits<uint32_t>::max();
#endif

  return safe_strnlen(str, max_len);
}
/** std string detection, ignoring the Allocator type **/
template <typename T>
struct is_std_string : std::false_type
{
};

template <typename Allocator>
struct is_std_string<std::basic_string<char, std::char_traits<char>, Allocator>> : std::true_type
{
};
} // namespace detail

/** typename = void for specializations with enable_if **/
template <typename Arg, typename = void>
struct Codec
{
  /***/
  QUILL_NODISCARD QUILL_ATTRIBUTE_HOT static size_t compute_encoded_size(
    QUILL_MAYBE_UNUSED detail::SizeCacheVector& conditional_arg_size_cache, QUILL_MAYBE_UNUSED Arg const& arg) noexcept
  {
    if constexpr (std::disjunction_v<std::is_arithmetic<Arg>, std::is_enum<Arg>,
                                     std::is_same<Arg, void const*>, std::is_same<Arg, void*>>)
    {
      return sizeof(Arg);
    }
    else if constexpr (std::conjunction_v<std::is_array<Arg>, std::is_same<detail::remove_cvref_t<std::remove_extent_t<Arg>>, char>>)
    {
      size_t constexpr N = std::extent_v<Arg>;
      size_t len = detail::safe_strnlen(arg, N) + 1u;
      if (QUILL_UNLIKELY(len > std::numeric_limits<uint32_t>::max()))
      {
        len = std::numeric_limits<uint32_t>::max();
      }
      return conditional_arg_size_cache.push_back(static_cast<uint32_t>(len));
    }
    else if constexpr (std::disjunction_v<std::is_same<Arg, char*>, std::is_same<Arg, char const*>>)
    {
      // for c strings we do an additional check for nullptr
      // include one extra for the zero termination
      size_t len = detail::safe_strnlen(arg) + 1u;
      if (QUILL_UNLIKELY(len > std::numeric_limits<uint32_t>::max()))
      {
        len = std::numeric_limits<uint32_t>::max();
      }
      return conditional_arg_size_cache.push_back(static_cast<uint32_t>(len));
    }
    else if constexpr (std::disjunction_v<detail::is_std_string<Arg>, std::is_same<Arg, std::string_view>>)
    {
      // for std::string we also need to store the size in order to correctly retrieve it
      // the reason for this is that if we create e.g:
      // std::string msg = fmtquill::format("{} {} {} {} {}", (char)0, (char)0, (char)0, (char)0,
      // "sssssssssssssssssssssss"); then strlen(msg.data()) = 0 but msg.size() = 31
      return sizeof(uint32_t) + static_cast<uint32_t>(arg.length());
    }
    else
    {
      detail::codec_not_found_for_type<Arg>();
      return 0;
    }
  }

  /***/
  QUILL_ATTRIBUTE_HOT static void encode(std::byte*& buffer,
                                         QUILL_MAYBE_UNUSED detail::SizeCacheVector const& conditional_arg_size_cache,
                                         QUILL_MAYBE_UNUSED uint32_t& conditional_arg_size_cache_index,
                                         Arg const& arg) noexcept
  {
    if constexpr (std::disjunction_v<std::is_arithmetic<Arg>, std::is_enum<Arg>,
                                     std::is_same<Arg, void const*>, std::is_same<Arg, void*>>)
    {
      std::memcpy(buffer, &arg, sizeof(Arg));
      buffer += sizeof(Arg);
    }
    else if constexpr (std::conjunction_v<std::is_array<Arg>, std::is_same<detail::remove_cvref_t<std::remove_extent_t<Arg>>, char>>)
    {
      size_t constexpr N = std::extent_v<Arg>;
      uint32_t const len = conditional_arg_size_cache[conditional_arg_size_cache_index++];

      // Local copy improves generated code (avoids aliasing penalties)
      std::byte* buf_ptr = buffer;

      if (QUILL_UNLIKELY(len > N))
      {
        // no '\0' in c array
        QUILL_ASSERT(len == N + 1, "Invalid length for c array in Codec::encode(), expected N + 1");
        std::memcpy(buf_ptr, arg, N);
        buf_ptr[N] = std::byte{'\0'};
      }
      else
      {
        std::memcpy(buf_ptr, arg, len);
      }

      buffer = buf_ptr + len;
    }
    else if constexpr (std::disjunction_v<std::is_same<Arg, char*>, std::is_same<Arg, char const*>>)
    {
      // null terminator is included in the len for c style strings
      uint32_t const len = conditional_arg_size_cache[conditional_arg_size_cache_index++];

      // Local copy improves generated code (avoids aliasing penalties)
      std::byte* buf_ptr = buffer;

      if (QUILL_LIKELY(arg != nullptr))
      {
        // avoid gcc warning, even when size == 0
        std::memcpy(buf_ptr, arg, len - 1);
      }

      buf_ptr[len - 1] = std::byte{'\0'};
      buffer = buf_ptr + len;
    }
    else if constexpr (std::disjunction_v<detail::is_std_string<Arg>, std::is_same<Arg, std::string_view>>)
    {
      // for std::string we store the size first, in order to correctly retrieve it
      // Copy the length first and then the actual string
      auto const len = static_cast<uint32_t>(arg.length());

      // Local copy improves generated code (avoids aliasing penalties)
      std::byte* buf_ptr = buffer;
      std::memcpy(buf_ptr, &len, sizeof(len));
      buf_ptr += sizeof(len);

      // Only copy if length > 0 to avoid UBSAN with nullptr in memcpy
      if (QUILL_LIKELY(len != 0))
      {
        std::memcpy(buf_ptr, arg.data(), len);
      }

      buffer = buf_ptr + len;
    }
    else
    {
      detail::codec_not_found_for_type<Arg>();
    }
  }

  // We use two separate functions, decode_arg and decode_and_store_arg, because there are
  // scenarios where we need to decode an argument without storing it in args_store, such as when
  // dealing with nested types. Storing the argument requires a fmtquill formatter, so having
  // two distinct functions allows us to avoid this requirement in cases where only decoding is needed.

  /***/
  static auto decode_arg(std::byte*& buffer)
  {
    if constexpr (std::disjunction_v<std::is_arithmetic<Arg>, std::is_enum<Arg>,
                                     std::is_same<Arg, void const*>, std::is_same<Arg, void*>>)
    {
      Arg arg;
      std::memcpy(&arg, buffer, sizeof(Arg));
      buffer += sizeof(Arg);
      return arg;
    }
    else if constexpr (std::disjunction_v<std::is_same<Arg, char*>, std::is_same<Arg, char const*>,
                                          std::conjunction<std::is_array<Arg>, std::is_same<detail::remove_cvref_t<std::remove_extent_t<Arg>>, char>>>)
    {
      // c strings or char array
      auto arg = reinterpret_cast<char const*>(buffer);
      // for c_strings we add +1 to the length as we also want to copy the null terminated char
      buffer += detail::safe_strnlen(arg) + 1u;
      return arg;
    }
    else if constexpr (std::disjunction_v<detail::is_std_string<Arg>, std::is_same<Arg, std::string_view>>)
    {
      // for std::string we first need to retrieve the length
      uint32_t len;
      std::memcpy(&len, buffer, sizeof(len));
      buffer += sizeof(len);
      auto const arg = std::string_view{reinterpret_cast<char const*>(buffer), len};
      buffer += len;
      return arg;
    }
    else
    {
      detail::codec_not_found_for_type<Arg>();
      return 0;
    }
  }

  /***/
  static void decode_and_store_arg(std::byte*& buffer, DynamicFormatArgStore* args_store)
  {
    if constexpr (std::disjunction_v<std::is_arithmetic<Arg>, std::is_enum<Arg>,
                                     std::is_same<Arg, void const*>, std::is_same<Arg, void*>>)
    {
      args_store->push_back(decode_arg(buffer));
    }
    else if constexpr (std::disjunction_v<std::is_same<Arg, char*>, std::is_same<Arg, char const*>,
                                          std::conjunction<std::is_array<Arg>, std::is_same<detail::remove_cvref_t<std::remove_extent_t<Arg>>, char>>>)
    {
      // c strings or char array
      char const* arg = decode_arg(buffer);

      // pass the string_view to args_store to avoid the dynamic allocation
      // we pass fmtquill::string_view since fmt/base.h includes a formatter for that type.
      // for std::string_view we would need fmt/format.h
      args_store->push_back(fmtquill::string_view{arg, detail::safe_strnlen(arg)});
    }
    else if constexpr (std::disjunction_v<detail::is_std_string<Arg>, std::is_same<Arg, std::string_view>>)
    {
      std::string_view arg = decode_arg(buffer);

      // pass the string_view to args_store to avoid the dynamic allocation
      // we pass fmtquill::string_view since fmt/base.h includes a formatter for that type.
      // for std::string_view we would need fmt/format.h
      args_store->push_back(fmtquill::string_view{arg.data(), arg.size()});
    }
    else
    {
      detail::codec_not_found_for_type<Arg>();
    }
  }
};

namespace detail
{
/**
 * @brief Calculates the total size required to encode the provided arguments

 * @param conditional_arg_size_cache Storage to avoid repeating calculations eg. cache strlen
 * @param args The arguments to be encoded.
 * @return The total size required to encode the arguments.
 */
template <typename... Args>
QUILL_NODISCARD QUILL_ATTRIBUTE_HOT size_t compute_encoded_size_and_cache_string_lengths(
  QUILL_MAYBE_UNUSED SizeCacheVector& conditional_arg_size_cache, Args const&... args)
{
  if constexpr (!std::conjunction_v<std::disjunction<std::is_arithmetic<remove_cvref_t<Args>>, std::is_same<remove_cvref_t<Args>, void const*>,
                                                     std::is_same<remove_cvref_t<Args>, void*>, is_std_string<remove_cvref_t<Args>>,
                                                     std::is_same<remove_cvref_t<Args>, std::string_view>>...>)
  {
    // Clear the cache if any argument type is not one of:
    // arithmetic, void*, std::string, or std::string_view.
    conditional_arg_size_cache.clear();
  }

  size_t total_sum{0};
  // Avoid using a fold expression with '+ ...' because we require a guaranteed evaluation
  // order to ensure that each argument is processed in sequence. This is essential for
  // correctly populating the conditional_arg_size_cache
  ((total_sum += Codec<remove_cvref_t<Args>>::compute_encoded_size(conditional_arg_size_cache, args)), ...);
  return total_sum;
}

/**
 * @brief Encodes multiple arguments into a buffer.
 * @param buffer Pointer to the buffer for encoding.
 * @param conditional_arg_size_cache Storage to avoid repeating calculations eg. cache strlen
 * @param args The arguments to be encoded.
 */
template <typename... Args>
QUILL_ATTRIBUTE_HOT void encode(std::byte*& buffer,
                                SizeCacheVector const& conditional_arg_size_cache, Args&&... args)
{
  QUILL_MAYBE_UNUSED uint32_t conditional_arg_size_cache_index{0};
  (Codec<remove_cvref_t<Args>>::encode(buffer, conditional_arg_size_cache, conditional_arg_size_cache_index,
                                       static_cast<decltype(args)&&>(args)),
   ...);
}

template <typename... Args>
void decode_and_store_arg(std::byte*& buffer, QUILL_MAYBE_UNUSED DynamicFormatArgStore* args_store)
{
  (Codec<Args>::decode_and_store_arg(buffer, args_store), ...);
}

/**
 * Decode functions
 */
using FormatArgsDecoder = void (*)(std::byte*& data, DynamicFormatArgStore& args_store);

template <typename... Args>
void decode_and_store_args(std::byte*& buffer, DynamicFormatArgStore& args_store)
{
  decode_and_store_arg<Args...>(buffer, &args_store);
}
} // namespace detail

/** Codec helpers for user defined types convenience **/
/***/
template <typename... TMembers>
size_t compute_total_encoded_size(detail::SizeCacheVector& conditional_arg_size_cache, TMembers const&... members)
{
  size_t total_size{0};
  ((total_size += Codec<detail::remove_cvref_t<TMembers>>::compute_encoded_size(conditional_arg_size_cache, members)),
   ...);
  return total_size;
}

/***/
template <typename... TMembers>
void encode_members(std::byte*& buffer, detail::SizeCacheVector const& conditional_arg_size_cache,
                    uint32_t& conditional_arg_size_cache_index, TMembers const&... members)
{
  ((Codec<detail::remove_cvref_t<TMembers>>::encode(buffer, conditional_arg_size_cache,
                                                    conditional_arg_size_cache_index, members)),
   ...);
}

/***/
template <typename T, typename... TMembers>
void decode_members(std::byte*& buffer, T&, TMembers&... members)
{
  // T& arg is not used but if we remove it, it will crash all users who are passing the extra argument without a compile time error
  ((members = Codec<detail::remove_cvref_t<TMembers>>::decode_arg(buffer)), ...);
}

QUILL_END_NAMESPACE
