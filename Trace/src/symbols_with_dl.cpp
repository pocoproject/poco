#ifdef CPPTRACE_GET_SYMBOLS_WITH_LIBDL

#include <cpptrace/cpptrace.hpp>
#include "symbols/symbols.hpp"

#include <cstdint>
#include <memory>
#include <vector>

#include <dlfcn.h>

namespace cpptrace {
namespace detail {
namespace libdl {
    stacktrace_frame resolve_frame(const frame_ptr addr) {
        Dl_info info;
        if(dladdr(reinterpret_cast<void*>(addr), &info)) { // thread-safe
            auto base = get_module_image_base(info.dli_fname);
            return {
                addr,
                base.has_value()
                    ? addr - reinterpret_cast<std::uintptr_t>(info.dli_fbase) + base.unwrap_value()
                    : 0,
                nullable<std::uint32_t>::null(),
                nullable<std::uint32_t>::null(),
                info.dli_fname ? info.dli_fname : "",
                info.dli_sname ? info.dli_sname : "",
                false
            };
        } else {
            return {
                addr,
                0,
                nullable<std::uint32_t>::null(),
                nullable<std::uint32_t>::null(),
                "",
                "",
                false
            };
        }
    }

    std::vector<stacktrace_frame> resolve_frames(const std::vector<frame_ptr>& frames) {
        std::vector<stacktrace_frame> trace;
        trace.reserve(frames.size());
        for(const auto frame : frames) {
            trace.push_back(resolve_frame(frame));
        }
        return trace;
    }
}
}
}

#endif
