#ifndef PE_HPP
#define PE_HPP

#include "utils/common.hpp"
#include "utils/utils.hpp"

#if IS_WINDOWS
#include <cstdint>
#include <string>

namespace cpptrace {
namespace detail {
    Result<std::uintptr_t, internal_error> pe_get_module_image_base(const std::string& object_path);
}
}

#endif

#endif
