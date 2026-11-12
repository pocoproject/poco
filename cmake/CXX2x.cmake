# Copyright (c) 2013 Nathan Osman

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# Determines whether the compiler supports the C++20 features that POCO uses
function(check_for_cxx20_compiler _VAR)
    message(STATUS "Checking for C++20 compiler")

    set(_test_source ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/test_compiler_cxx20.cpp)

    set(${_VAR} PARENT_SCOPE)
    try_compile(POCO_CXX20_COMPILER_CHECK ${PROJECT_BINARY_DIR} ${_test_source}
        CMAKE_FLAGS -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} -DCMAKE_CXX_STANDARD_REQUIRED=ON
        OUTPUT_VARIABLE _test_output)
    if(NOT POCO_CXX20_COMPILER_CHECK)
        message(STATUS "Checking for C++20 compiler - unavailable")
        message(STATUS "${_test_output}")
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            message(STATUS "C++20 with the default standard library failed; to use libc++, configure with -DCMAKE_CXX_FLAGS=-stdlib=libc++")
        endif()
        return()
    endif()

    set(${_VAR} 1 PARENT_SCOPE)
    message(STATUS "Checking for C++20 compiler - available")

    # Oldest compilers that POCO is tested with. Emscripten is versioned by its SDK, not by its Clang.
    set(_min_GNU 12)
    set(_min_Clang 16)
    set(_min_AppleClang 16)
    set(_min_MSVC 19.30)
    if(NOT EMSCRIPTEN AND DEFINED _min_${CMAKE_CXX_COMPILER_ID} AND
        CMAKE_CXX_COMPILER_VERSION VERSION_LESS "${_min_${CMAKE_CXX_COMPILER_ID}}")
        message(WARNING "POCO is tested with GCC 12, Clang 16, Apple Clang 16 and Visual C++ 2022 or newer; "
            "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} is older and not tested.")
    endif()
endfunction()
