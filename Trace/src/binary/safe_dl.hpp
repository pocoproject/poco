#ifndef SAFE_DL_HPP
#define SAFE_DL_HPP

#include "utils/common.hpp"

namespace cpptrace {
namespace detail {
    void get_safe_object_frame(frame_ptr address, safe_object_frame* out);
}
}

#endif
