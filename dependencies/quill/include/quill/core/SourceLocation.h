#pragma once

/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include <cstdint>

#if !defined(QUILL_BUILTIN_FUNCTION_NAME)
  #if defined(QUILL_DISABLE_FUNCTION_NAME)
    #define QUILL_BUILTIN_FUNCTION_NAME ""
  #else
    #define QUILL_BUILTIN_FUNCTION_NAME __builtin_FUNCTION()
  #endif
#endif

#if !defined(QUILL_BUILTIN_FILE_NAME)
  #if defined(QUILL_DISABLE_FILE_INFO)
    #define QUILL_BUILTIN_FILE_NAME ""
  #else
    #define QUILL_BUILTIN_FILE_NAME __builtin_FILE()
  #endif
#endif

#if !defined(QUILL_BUILTIN_LINE_NO)
  #if defined(QUILL_DISABLE_FILE_INFO)
    #define QUILL_BUILTIN_LINE_NO 0u
  #else
    #define QUILL_BUILTIN_LINE_NO __builtin_LINE()
  #endif
#endif

QUILL_BEGIN_NAMESPACE

struct SourceLocation
{
  static constexpr SourceLocation current(char const* file = QUILL_BUILTIN_FILE_NAME,
                                          char const* function = QUILL_BUILTIN_FUNCTION_NAME,
                                          std::uint_least32_t line = QUILL_BUILTIN_LINE_NO) noexcept
  {
    return SourceLocation{file, function, line};
  }

  constexpr SourceLocation(char const* file, char const* function, std::uint_least32_t line)
    : _file(file), _function(function), _line(line)
  {
  }

  QUILL_NODISCARD constexpr const char* file_name() const noexcept { return _file; }
  QUILL_NODISCARD constexpr const char* function_name() const noexcept { return _function; }
  QUILL_NODISCARD constexpr std::uint_least32_t line() const noexcept { return _line; }

private:
  char const* _file;
  char const* _function;
  std::uint_least32_t _line;
};

QUILL_END_NAMESPACE