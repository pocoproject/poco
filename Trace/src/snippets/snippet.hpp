#ifndef SNIPPET_HPP
#define SNIPPET_HPP

#include <cstddef>
#include <string>

namespace cpptrace {
namespace detail {
    // 1-indexed line
    std::string get_snippet(const std::string& path, std::size_t line, std::size_t context_size, bool color);
}
}

#endif
