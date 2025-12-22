/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"

#include <cstddef>
#include <cstdint>

#if defined(QUILL_ENABLE_ASSERTIONS) || !defined(NDEBUG)

  #include <cstdio>
  #include <cstdlib>

#endif

/**
 * Convert number to string
 */
#define QUILL_AS_STR(x) #x
#define QUILL_STRINGIFY(x) QUILL_AS_STR(x)

#if !defined(QUILL_THREAD_LOCAL)
  #if defined(__GNUC__) && defined(__linux__)
    #define QUILL_THREAD_LOCAL __thread
  #else
    #define QUILL_THREAD_LOCAL thread_local
  #endif
#endif

#if !defined(QUILL_MAGIC_SEPARATOR)
  #define QUILL_MAGIC_SEPARATOR "\x01\x02\x03"
#endif

#if !defined(QUILL_FUNCTION_NAME)
  #if defined(QUILL_DISABLE_FUNCTION_NAME) && defined(QUILL_DETAILED_FUNCTION_NAME)
    #error "QUILL_DISABLE_FUNCTION_NAME and QUILL_DETAILED_FUNCTION_NAME are mutually exclusive"
  #endif

  #if defined(QUILL_DISABLE_FUNCTION_NAME)
    #define QUILL_FUNCTION_NAME ""
  #elif defined(QUILL_DETAILED_FUNCTION_NAME)
    #if defined(_MSC_VER)
      #define QUILL_FUNCTION_NAME __FUNCSIG__
    #elif defined(__clang__) || defined(__GNUC__) || defined(__INTEL_COMPILER)
      #define QUILL_FUNCTION_NAME __PRETTY_FUNCTION__
    #else
      #define QUILL_FUNCTION_NAME __FUNCTION__
    #endif
  #else
    #define QUILL_FUNCTION_NAME __FUNCTION__
  #endif
#endif

#if !defined(QUILL_FILE_NAME)
  #if defined(QUILL_DISABLE_FILE_INFO)
    #define QUILL_FILE_NAME ""
  #else
    #define QUILL_FILE_NAME __FILE__
  #endif
#endif

#if !defined(QUILL_LINE_NO)
  #if defined(QUILL_DISABLE_FILE_INFO)
    #define QUILL_LINE_NO 0
  #else
    #define QUILL_LINE_NO __LINE__
  #endif
#endif

#if !defined(QUILL_FILE_INFO)
  #if defined(QUILL_DISABLE_FILE_INFO)
    #define QUILL_FILE_INFO ""
  #else
    #define QUILL_FILE_INFO QUILL_FILE_NAME ":" QUILL_STRINGIFY(QUILL_LINE_NO)
  #endif
#endif

#if !defined(QUILL_ASSERT)
  #if defined(QUILL_ENABLE_ASSERTIONS) || !defined(NDEBUG)
    #define QUILL_ASSERT(expr, msg)                                                                \
      do                                                                                           \
      {                                                                                            \
        if QUILL_UNLIKELY (!(expr))                                                                \
        {                                                                                          \
          std::fprintf(stderr, "Quill assertion failed: %s, file %s, line %d. %s\n", #expr,        \
                       __FILE__, __LINE__, msg);                                                   \
          std::abort();                                                                            \
        }                                                                                          \
      } while (0)
  #else
    #define QUILL_ASSERT(expr, msg) ((void)0)
  #endif
#endif

#if !defined(QUILL_ASSERT_WITH_FMT)
  #if defined(QUILL_ENABLE_ASSERTIONS) || !defined(NDEBUG)
    #define QUILL_ASSERT_WITH_FMT(expr, fmt, ...)                                                  \
      do                                                                                           \
      {                                                                                            \
        if QUILL_UNLIKELY (!(expr))                                                                \
        {                                                                                          \
          char quill_assert_msg_buf[512];                                                          \
          std::snprintf(quill_assert_msg_buf, sizeof(quill_assert_msg_buf), fmt, __VA_ARGS__);     \
          std::fprintf(stderr, "Quill assertion failed: %s, file %s, line %d. %s\n", #expr,        \
                       __FILE__, __LINE__, quill_assert_msg_buf);                                  \
          std::abort();                                                                            \
        }                                                                                          \
      } while (0)
  #else
    #define QUILL_ASSERT_WITH_FMT(expr, fmt, ...) ((void)0)
  #endif
#endif

QUILL_BEGIN_NAMESPACE

namespace detail
{
/**
 * Cache line constants.
 *
 * Note: On FreeBSD, CACHE_LINE_SIZE is defined in a system header,
 * so we use a prefix to avoid naming conflicts.
 */
static constexpr size_t QUILL_CACHE_LINE_SIZE{64u};
static constexpr size_t QUILL_CACHE_LINE_ALIGNED{2 * QUILL_CACHE_LINE_SIZE};

// define our own PATH_PREFERRED_SEPARATOR to not include <filesystem>
#if defined(_WIN32) && defined(_MSC_VER) && !defined(__GNUC__)
// Regular Windows (MSVC, MinGW): use backslashes
static constexpr wchar_t PATH_PREFERRED_SEPARATOR = L'\\';
#else
// Non-Windows OR Clang on Windows: use forward slashes
static constexpr char PATH_PREFERRED_SEPARATOR = '/';
#endif
} // namespace detail

/**
 * Enum to select a queue type
 */
enum class QueueType
{
  UnboundedBlocking,
  UnboundedDropping,
  BoundedBlocking,
  BoundedDropping
};

/**
 * Enum to select a timezone
 */
enum class Timezone : uint8_t
{
  LocalTime,
  GmtTime
};

/**
 * Enum for the used clock type
 */
enum class ClockSourceType : uint8_t
{
  Tsc,
  System,
  User
};

/**
 * Enum for huge pages
 */
enum class HugePagesPolicy
{
  Never,  // Do not use huge pages
  Always, // Use huge pages, fail if unavailable
  Try     // Try huge pages, but fall back to normal pages if unavailable
};

QUILL_END_NAMESPACE
