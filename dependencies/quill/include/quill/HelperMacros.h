/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/DeferredFormatCodec.h"
#include "quill/DirectFormatCodec.h"
#include "quill/bundled/fmt/format.h"
#include "quill/bundled/fmt/ostream.h"
#include <type_traits>

/**
 * Convenient macro to define a formatter for a user defined type.
 * This macro will define a formatter for the type and a codec for the type.
 * The formatter will use the fmt::ostream_formatter and the codec will use the DeferredFormatCodec.
 */
#define QUILL_LOGGABLE_DEFERRED_FORMAT(type)                                                       \
  template <>                                                                                      \
  struct fmtquill::formatter<type> : fmtquill::ostream_formatter                                   \
  {                                                                                                \
  };                                                                                               \
                                                                                                   \
  template <>                                                                                      \
  struct quill::Codec<type> : quill::DeferredFormatCodec<type>                                     \
  {                                                                                                \
  };

/**
 * Convenient macro to define a formatter for a user defined type.
 * This macro will define a formatter for the type and a codec for the type.
 * The formatter will use the fmt::ostream_formatter and the codec will use the DirectFormatCodec.
 */
#define QUILL_LOGGABLE_DIRECT_FORMAT(type)                                                         \
  template <>                                                                                      \
  struct fmtquill::formatter<type> : fmtquill::ostream_formatter                                   \
  {                                                                                                \
  };                                                                                               \
                                                                                                   \
  template <>                                                                                      \
  struct quill::Codec<type> : quill::DirectFormatCodec<type>                                       \
  {                                                                                                \
  };
