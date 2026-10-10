/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#ifndef QUILL_BEGIN_NAMESPACE
  #define QUILL_BEGIN_NAMESPACE                                                                    \
    namespace quill                                                                                \
    {                                                                                              \
    inline namespace v11                                                                           \
    {
  #define QUILL_END_NAMESPACE                                                                      \
    }                                                                                              \
    }
#endif

#ifdef _MSVC_LANG
  #define QUILL_CPLUSPLUS _MSVC_LANG
#else
  #define QUILL_CPLUSPLUS __cplusplus
#endif

/**
 * __has_include
 */
#ifndef QUILL_HAS_INCLUDE
  #ifdef __has_include
    #define QUILL_HAS_INCLUDE(x) __has_include(x)
  #else
    #define QUILL_HAS_INCLUDE(x) 0
  #endif
#endif

/**
 * __has_feature
 */
#ifndef QUILL_HAS_FEATURE
  #ifdef __has_feature
    #define QUILL_HAS_FEATURE(x) __has_feature(x)
  #else
    #define QUILL_HAS_FEATURE(x) 0
  #endif
#endif

/**
 * __has_attribute
 */
#ifndef QUILL_HAS_ATTRIBUTE
  #ifdef __has_attribute
    #define QUILL_HAS_ATTRIBUTE(x) __has_attribute(x)
  #else
    #define QUILL_HAS_ATTRIBUTE(x) 0
  #endif
#endif

/**
 * __has_cpp_attribute
 */
#ifndef QUILL_HAS_CPP_ATTRIBUTE
  #if defined(__cplusplus) && defined(__has_cpp_attribute)
    #define QUILL_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
  #else
    #define QUILL_HAS_CPP_ATTRIBUTE(x) 0
  #endif
#endif

/**
 * Portable no discard warnings
 */
#ifndef QUILL_NODISCARD
  #if QUILL_HAS_CPP_ATTRIBUTE(nodiscard)
    #define QUILL_NODISCARD [[nodiscard]]
  #elif QUILL_HAS_CPP_ATTRIBUTE(gnu::warn_unused_result)
    #define QUILL_NODISCARD [[gnu::warn_unused_result]]
  #else
    #define QUILL_NODISCARD
  #endif
#endif

/**
 * Portable maybe_unused
 */
#ifndef QUILL_MAYBE_UNUSED
  #if QUILL_HAS_CPP_ATTRIBUTE(maybe_unused) && (__cplusplus >= 201703L)
    #define QUILL_MAYBE_UNUSED [[maybe_unused]]
  #elif QUILL_HAS_ATTRIBUTE(__unused__)
    #define QUILL_MAYBE_UNUSED __attribute__((__unused__))
  #elif defined(_MSC_VER)
    #define QUILL_MAYBE_UNUSED __pragma(warning(suppress : 4100))
  #else
    #define QUILL_MAYBE_UNUSED
  #endif
#endif

/**
 * Gcc hot/cold attributes
 * Tells GCC that a function is hot or cold. GCC can use this information to
 * improve static analysis, i.e. a conditional branch to a cold function
 * is likely to be not-taken.
 */
#ifndef QUILL_ATTRIBUTE_HOT
  #if QUILL_HAS_ATTRIBUTE(hot)
    #define QUILL_ATTRIBUTE_HOT __attribute__((hot))
  #else
    #define QUILL_ATTRIBUTE_HOT
  #endif
#endif

#ifndef QUILL_ATTRIBUTE_COLD
  #if QUILL_HAS_ATTRIBUTE(cold)
    #define QUILL_ATTRIBUTE_COLD __attribute__((cold))
  #else
    #define QUILL_ATTRIBUTE_COLD
  #endif
#endif

/**
 * Used
 */
#ifndef QUILL_ATTRIBUTE_USED
  #if QUILL_HAS_ATTRIBUTE(used)
    #define QUILL_ATTRIBUTE_USED __attribute__((used))
  #else
    #define QUILL_ATTRIBUTE_USED
  #endif
#endif

/**
 * Likely
 */
#ifndef QUILL_LIKELY
  #if defined(__GNUC__)
    #define QUILL_LIKELY(x) (__builtin_expect((x), 1))
  #else
    #define QUILL_LIKELY(x) (x)
  #endif
#endif

#ifndef QUILL_UNLIKELY
  #if defined(__GNUC__)
    #define QUILL_UNLIKELY(x) (__builtin_expect((x), 0))
  #else
    #define QUILL_UNLIKELY(x) (x)
  #endif
#endif

/**
 * Visibility
 */
#ifndef QUILL_EXPORT
  #if defined(_WIN32) && defined(_MSC_VER)
    #if defined(QUILL_DLL_EXPORT)
      #define QUILL_EXPORT __declspec(dllexport) // Exporting symbols when building the library
    #elif defined(QUILL_DLL_IMPORT)
      #define QUILL_EXPORT __declspec(dllimport) // Importing symbols when using the library
    #else
      #define QUILL_EXPORT // No special attribute needed for static or other builds
    #endif
  #elif defined(__GNUC__) || defined(__clang__)
    #define QUILL_EXPORT                                                                           \
      __attribute__((visibility("default"))) // Using GCC/Clang visibility attribute
  #else
    #define QUILL_EXPORT // Default for other compilers
  #endif
#endif

/**
 * Check RTTI is available
 */
#ifndef QUILL_USE_RTTI
  // __RTTI is for EDG compilers. _CPPRTTI is for MSVC.
  #if defined(__GXX_RTTI) || QUILL_HAS_FEATURE(cxx_rtti) || defined(_CPPRTTI) ||                   \
    defined(__INTEL_RTTI__) || defined(__RTTI)
    #define QUILL_USE_RTTI 1
  #else
    #define QUILL_USE_RTTI 0
  #endif
#endif