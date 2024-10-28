#ifndef IMAGE_MODULE_BASE_HPP
#define IMAGE_MODULE_BASE_HPP

#include "utils/common.hpp"
#include "utils/utils.hpp"

#include <cstdint>
#include <string>

namespace cpptrace {
namespace detail {
    Result<std::uintptr_t, internal_error> get_module_image_base(const std::string& object_path);
}
}

#endif
