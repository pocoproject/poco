/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/QuillError.h"

#include <cerrno>
#include <cstring>
#include <ctime>
#include <string>

// Forward declaration
struct tm;

QUILL_BEGIN_NAMESPACE

namespace detail
{
/**
 * Portable gmtime_r or _s per operating system
 * @param timer to a time_t object to convert
 * @param buf to a struct tm object to store the result
 * @return copy of the buf pointer, or throws on error
 * @throws std::system_error on failure
 */
inline tm* gmtime_rs(time_t const* timer, tm* buf)
{
#if defined(_WIN32)
  errno_t const res = gmtime_s(buf, timer);
  if (res)
  {
    QUILL_THROW(QuillError{
      std::string{"failed to call gmtime_rs, with error message errno: " + std::to_string(res)}});
  }
  return buf;
#else
  tm* res = gmtime_r(timer, buf);
  if (QUILL_UNLIKELY(!res))
  {
    QUILL_THROW(QuillError{std::string{"failed to call gmtime_rs, with error message errno: " +
                                       std::to_string(errno) + " error: " + std::strerror(errno)}});
  }
  return res;
#endif
}

/**
 * Portable localtime_r or _s per operating system
 * @param timer to a time_t object to convert
 * @param buf to a struct tm object to store the result
 * @return copy of the buf pointer, or throws on error
 * @throws std::system_error on failure
 */
inline tm* localtime_rs(time_t const* timer, tm* buf)
{
#if defined(_WIN32)
  auto const res = localtime_s(buf, timer);
  if (res)
  {
    QUILL_THROW(QuillError{
      std::string{"failed to call localtime_rs, with error message errno: " + std::to_string(res)}});
  }
  return buf;
#else
  tm* res = localtime_r(timer, buf);
  if (QUILL_UNLIKELY(!res))
  {
    QUILL_THROW(QuillError{std::string{"failed to call localtime_rs, with error message errno: " +
                                       std::to_string(errno) + " error: " + std::strerror(errno)}});
  }
  return res;
#endif
}

/**
 * inverses of gmtime
 * @param tm struct tm to convert
 * @throws on invalid input
 */
inline time_t timegm(tm* tm)
{
#if defined(_WIN32)
  time_t const ret_val = _mkgmtime(tm);

  if (QUILL_UNLIKELY(ret_val == -1))
  {
    QUILL_THROW(QuillError{"_mkgmtime failed."});
  }

  return ret_val;
#else
  time_t const ret_val = ::timegm(tm);

  if (QUILL_UNLIKELY(ret_val == (time_t)-1))
  {
    QUILL_THROW(QuillError{"timegm failed."});
  }

  return ret_val;
#endif
}
} // namespace detail

QUILL_END_NAMESPACE