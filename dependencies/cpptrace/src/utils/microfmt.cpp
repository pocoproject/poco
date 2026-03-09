#include "utils/microfmt.hpp"

#include <iostream>

CPPTRACE_BEGIN_NAMESPACE
namespace microfmt::detail {

    std::ostream& get_cout() {
        return std::cout;
    }

} // namespace microfmt::detail
CPPTRACE_END_NAMESPACE
