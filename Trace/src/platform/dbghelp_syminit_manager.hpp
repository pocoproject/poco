#ifndef DBGHELP_SYMINIT_MANAGER_HPP
#define DBGHELP_SYMINIT_MANAGER_HPP

#include "utils/common.hpp"
#include "utils/utils.hpp"

#include <unordered_set>

#include <windows.h>
#include <dbghelp.h>

namespace cpptrace {
namespace detail {
    struct dbghelp_syminit_manager {
        std::unordered_set<HANDLE> set;

        ~dbghelp_syminit_manager() {
            for(auto handle : set) {
                if(!SymCleanup(handle)) {
                    ASSERT(false, microfmt::format("Cpptrace SymCleanup failed with code {}\n", GetLastError()).c_str());
                }
            }
        }

        void init(HANDLE proc) {
            if(set.count(proc) == 0) {
                if(!SymInitialize(proc, NULL, TRUE)) {
                    throw internal_error("SymInitialize failed {}", GetLastError());
                }
                set.insert(proc);
            }
        }
    };

    // Thread-safety: Must only be called from symbols_with_dbghelp while the dbghelp_lock lock is held
    inline dbghelp_syminit_manager& get_syminit_manager() {
        static dbghelp_syminit_manager syminit_manager;
        return syminit_manager;
    }
}
}

#endif
