#ifndef PATH_HPP
#define PATH_HPP

#include "utils/common.hpp"
#include "platform/platform.hpp"

#if IS_WINDOWS
#include <windows.h>
#endif

namespace cpptrace {
namespace detail {
    #if IS_WINDOWS
    constexpr char PATH_SEP = '\\';
    inline bool is_absolute(const std::string& path) {
        // I don't want to bring in shlwapi as a dependency just for PathIsRelativeA so I'm following the guidance of
        // https://stackoverflow.com/a/71941552/15675011 and
        // https://github.com/wine-mirror/wine/blob/b210a204137dec8d2126ca909d762454fd47e963/dlls/kernelbase/path.c#L982
        if(path.empty() || IsDBCSLeadByte(path[0])) {
            return false;
        }
        if(path[0] == '\\') {
            return true;
        }
        if(path.size() >= 2 && std::isalpha(path[0]) && path[1] == ':') {
            return true;
        }
        return false;
    }
    #else
    constexpr char PATH_SEP = '/';
    inline bool is_absolute(const std::string& path) {
        if(path.empty()) {
            return false;
        }
        return path[0] == '/';
    }
    #endif
}
}

#endif
