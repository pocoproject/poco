#ifndef UNWIND_HPP
#define UNWIND_HPP

#include "utils/common.hpp"
#include "utils/utils.hpp"

#include <cstddef>
#include <vector>

namespace cpptrace {
namespace detail {
    #ifdef CPPTRACE_HARD_MAX_FRAMES
    constexpr std::size_t hard_max_frames = CPPTRACE_HARD_MAX_FRAMES;
    #else
    constexpr std::size_t hard_max_frames = 200;
    #endif

    CPPTRACE_FORCE_NO_INLINE
    std::vector<frame_ptr> capture_frames(std::size_t skip, std::size_t max_depth);

    CPPTRACE_FORCE_NO_INLINE
    std::size_t safe_capture_frames(frame_ptr* buffer, std::size_t size, std::size_t skip, std::size_t max_depth);

    bool has_safe_unwind();
}
}

#endif
