# Detect whether linking against libatomic is required for std::atomic.
# Sets Poco_ATOMIC_LIBRARIES (empty if not needed, "atomic" if needed).

include(CheckCXXSourceCompiles)
include(CheckLibraryExists)

set(Poco_ATOMIC_LIBRARIES "")

# MSVC and Apple don't ship/need a separate libatomic.
if(NOT MSVC AND NOT APPLE)
    # The atomic variables in the test programs are global and modified with
    # a read-modify-write operation, otherwise the compiler can fold them away
    # when optimizing and the check passes even though libatomic is needed.

    # 1) 32-bit atomics
    check_cxx_source_compiles("
	#include <atomic>
	std::atomic<int> x(0);
	int main(int argc, char **) {
	    x.fetch_add(argc);
	    return x.load();
	}
    " POCO_HAVE_CXX_ATOMICS32)

    if(NOT POCO_HAVE_CXX_ATOMICS32)
	check_library_exists(atomic __atomic_fetch_add_4 "" POCO_HAVE_LIBATOMIC)
	if(POCO_HAVE_LIBATOMIC)
	    set(Poco_ATOMIC_LIBRARIES "atomic")
	endif()
    endif()

    # 2) 64-bit atomics (the common trigger on 32-bit ARM, MIPS, etc.)
    check_cxx_source_compiles("
	#include <atomic>
	#include <cstdint>
	std::atomic<uint64_t> x(0);
	int main(int argc, char **) {
	    x.fetch_add(static_cast<uint64_t>(argc));
	    return static_cast<int>(x.load());
	}
    " POCO_HAVE_CXX_ATOMICS64)

    if(NOT POCO_HAVE_CXX_ATOMICS64)
	check_library_exists(atomic __atomic_load_8 "" POCO_HAVE_LIBATOMIC64)
	if(POCO_HAVE_LIBATOMIC64)
	    set(Poco_ATOMIC_LIBRARIES "atomic")
	endif()
    endif()
endif()
