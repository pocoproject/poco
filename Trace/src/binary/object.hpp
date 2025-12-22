#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "utils/common.hpp"
#include "utils/utils.hpp"
#include "binary/module_base.hpp"

#include <string>
#include <vector>

namespace cpptrace {
namespace detail {
    object_frame get_frame_object_info(frame_ptr address);

    std::vector<object_frame> get_frames_object_info(const std::vector<frame_ptr>& addresses);

    object_frame resolve_safe_object_frame(const safe_object_frame& frame);
}
}

#endif
