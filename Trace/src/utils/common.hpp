#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstdio>
#include <stdexcept>
#include <string>

#include <cpptrace/cpptrace.hpp>

#include "platform/platform.hpp"

#define ESC     "\033["
#define RESET   ESC "0m"
#define RED     ESC "31m"
#define GREEN   ESC "32m"
#define YELLOW  ESC "33m"
#define BLUE    ESC "34m"
#define MAGENTA ESC "35m"
#define CYAN    ESC "36m"

#if IS_GCC || IS_CLANG
 #define NODISCARD __attribute__((warn_unused_result))
// #elif IS_MSVC && _MSC_VER >= 1700
//  #define NODISCARD _Check_return_
#else
 #define NODISCARD
#endif

namespace cpptrace {
namespace detail {
    static const stacktrace_frame null_frame {
        0,
        0,
        nullable<uint32_t>::null(),
        nullable<uint32_t>::null(),
        "",
        "",
        false
    };

    bool should_absorb_trace_exceptions();
    bool should_resolve_inlined_calls();
    enum cache_mode get_cache_mode();
}
}

#endif
