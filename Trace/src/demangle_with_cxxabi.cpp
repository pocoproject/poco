#ifdef CPPTRACE_DEMANGLE_WITH_CXXABI

#include "demangle/demangle.hpp"

#include <cxxabi.h>

#include <cstdlib>
#include <string>

namespace cpptrace {
namespace detail {
    std::string demangle(const std::string& name) {
        int status;
        // presumably thread-safe
        // it appears safe to pass nullptr for status however the docs don't explicitly say it's safe so I don't
        // want to rely on it
        char* const demangled = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status);
        // demangled will always be nullptr on non-zero status, and if __cxa_demangle ever fails for any reason
        // we'll just quietly return the mangled name
        if(demangled) {
            std::string str = demangled;
            std::free(demangled);
            return str;
        } else {
            return name;
        }
    }
}
}

#endif
