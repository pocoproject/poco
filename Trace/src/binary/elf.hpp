#ifndef ELF_HPP
#define ELF_HPP

#include "utils/common.hpp"
#include "utils/utils.hpp"

#if IS_LINUX

#include <cstdint>
#include <string>

namespace cpptrace {
namespace detail {
    Result<std::uintptr_t, internal_error> elf_get_module_image_base(const std::string& object_path);
}
}

#endif

#endif
