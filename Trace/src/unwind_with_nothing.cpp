#ifdef CPPTRACE_UNWIND_WITH_NOTHING

#include "unwind/unwind.hpp"

#include <cstddef>
#include <vector>

namespace cpptrace {
namespace detail {
    std::vector<frame_ptr> capture_frames(std::size_t, std::size_t) {
        return {};
    }

    CPPTRACE_FORCE_NO_INLINE
    std::size_t safe_capture_frames(frame_ptr*, std::size_t, std::size_t, std::size_t) {
        return 0;
    }

    bool has_safe_unwind() {
        return false;
    }
}
}

#endif
