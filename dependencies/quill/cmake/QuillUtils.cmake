include(CheckCXXSourceCompiles)

# Get Quill version from include/quill/Version.h and store it as QUILL_VERSION
function(quill_extract_version)
    file(READ "${CMAKE_CURRENT_LIST_DIR}/include/quill/Backend.h" file_contents)

    string(REGEX MATCH "constexpr uint32_t VersionMajor{([0-9]+)}" _ "${file_contents}")
    if (NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Failed to extract major version number from quill/Backend.h")
    endif ()
    set(version_major ${CMAKE_MATCH_1})

    string(REGEX MATCH "constexpr uint32_t VersionMinor{([0-9]+)}" _ "${file_contents}")
    if (NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Failed to extract minor version number from quill/Backend.h")
    endif ()
    set(version_minor ${CMAKE_MATCH_1})

    string(REGEX MATCH "constexpr uint32_t VersionPatch{([0-9]+)}" _ "${file_contents}")
    if (NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Failed to extract patch version number from quill/Backend.h")
    endif ()
    set(version_patch ${CMAKE_MATCH_1})

    set(QUILL_VERSION "${version_major}.${version_minor}.${version_patch}" PARENT_SCOPE)
endfunction()

# Define the function to set common compile options
function(set_common_compile_options target_name)
    cmake_parse_arguments(COMPILE_OPTIONS "" "VISIBILITY" "" ${ARGN})

    # Set default visibility to PRIVATE if not provided
    if (NOT DEFINED COMPILE_OPTIONS_VISIBILITY)
        set(COMPILE_OPTIONS_VISIBILITY PRIVATE)
    endif ()

    target_compile_options(${target_name} ${COMPILE_OPTIONS_VISIBILITY}
            # General warnings for Clang, AppleClang, and GNU, but NOT on Windows
            $<$<AND:$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>,$<NOT:$<PLATFORM_ID:Windows>>>:
            -Wall -Wextra -pedantic -Werror -Wredundant-decls -Wfloat-equal
            >

            # GCC-specific hardening and security flags
            $<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${QUILL_ENABLE_GCC_HARDENING}>>:
            -fstack-protector-strong
            -fstack-clash-protection
            -Wformat
            -Werror=format-security
            -fcf-protection
            -Wdate-time
            -D_FORTIFY_SOURCE=2
            >

            # GCC >= 7.1 specific: suppress PSABI warning
            $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,7.1>>:
            -Wno-psabi
            >

            # Clang specific options, but NOT on Windows
            $<$<AND:$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<NOT:$<PLATFORM_ID:Windows>>>:
            -Wimplicit-int-float-conversion -Wdocumentation
            >

            # Disable C++20 extension warnings for Clang > 17, but NOT on Windows
            $<$<AND:$<CXX_COMPILER_ID:Clang>,$<VERSION_GREATER:$<CXX_COMPILER_VERSION>,17>,$<NOT:$<PLATFORM_ID:Windows>>>:
            -Wno-c++20-extensions
            >

            # Disable specific warning for Clang and AppleClang, but NOT on Windows
            $<$<AND:$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,$<NOT:$<PLATFORM_ID:Windows>>>:
            -Wno-gnu-zero-variadic-macro-arguments
            >

            # Windows-specific options
            $<$<PLATFORM_ID:Windows>:$<$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Clang>>:/bigobj /WX /W4>>
    )

    if (QUILL_NO_EXCEPTIONS)
        # Modify CMake's default flags for MSVC to remove /EHsc
        if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        endif ()

        # Add flags -fno-exceptions -fno-rtti to make sure we support them
        target_compile_options(${target_name} ${COMPILE_OPTIONS_VISIBILITY}
                $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                -fno-exceptions -fno-rtti>
                $<$<CXX_COMPILER_ID:MSVC>:/wd4702 /GR- /EHs-c- /D_HAS_EXCEPTIONS=0>)
    else ()
        # Additional MSVC specific options - only set for non-QUILL_NO_EXCEPTIONS builds
        target_compile_options(${target_name} ${COMPILE_OPTIONS_VISIBILITY}
                $<$<CXX_COMPILER_ID:MSVC>:/EHsc>)
    endif ()
endfunction()

function(check_cxx_atomics_available variable)
  set(SAVED_CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES}")
  set(CMAKE_REQUIRED_LIBRARIES "")

  check_cxx_source_compiles("
    #include <atomic>
    #include <cstdint>
    std::atomic<uint64_t> counter;
    int main() {
      uint64_t res = std::atomic_fetch_add(&counter, 1);
      return (int)res;
    }" ${variable})

  set(CMAKE_REQUIRED_LIBRARIES "${SAVED_CMAKE_REQUIRED_LIBRARIES}")
endfunction()
