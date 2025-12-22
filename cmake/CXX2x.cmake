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

# Determines whether the compiler supports C++20
function(check_for_cxx20_compiler _VAR)
    message(STATUS "Checking for C++20 compiler")

    set(_test_source ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/test_compiler_cxx20.cpp)

    set(${_VAR} PARENT_SCOPE)
    try_compile(_COMPILER_TEST_RESULT ${PROJECT_BINARY_DIR} ${_test_source} CMAKE_FLAGS -DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_STANDARD_REQUIRED=ON)
    if(NOT _COMPILER_TEST_RESULT AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        try_compile(
            _COMPILER_TEST_RESULT ${PROJECT_BINARY_DIR} ${_test_source}
            CMAKE_FLAGS
                -DCMAKE_CXX_FLAGS="-stdlib=libc++" -DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_STANDARD_REQUIRED=ON
        )
        if(_COMPILER_TEST_RESULT)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        else()
            message(STATUS "To enable C++20 install libc++ standard library from https://libcxx.llvm.org/")
        endif()
    endif()

    if(_COMPILER_TEST_RESULT AND ((MSVC AND (MSVC14)) OR
    (CMAKE_COMPILER_IS_GNUCXX AND NOT ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 10.0) OR
    (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 10.0) OR
    (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" AND NOT ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 13.0)))
        set(${_VAR} 1 PARENT_SCOPE)
        message(STATUS "Checking for C++20 compiler - available")
    else()
        message(STATUS "Checking for C++20 compiler - unavailable")
    endif()
endfunction()
