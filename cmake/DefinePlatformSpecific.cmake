# http://www.cmake.org/Wiki/CMake_Useful_Variables :
# CMAKE_BUILD_TYPE
#    Choose the type of build. CMake has default flags for these:
#
#    * None (CMAKE_C_FLAGS or CMAKE_CXX_FLAGS used)
#    * Debug (CMAKE_C_FLAGS_DEBUG or CMAKE_CXX_FLAGS_DEBUG)
#    * Release (CMAKE_C_FLAGS_RELEASE or CMAKE_CXX_FLAGS_RELEASE)
#    * RelWithDebInfo (CMAKE_C_FLAGS_RELWITHDEBINFO or CMAKE_CXX_FLAGS_RELWITHDEBINFO
#    * MinSizeRel (CMAKE_C_FLAGS_MINSIZEREL or CMAKE_CXX_FLAGS_MINSIZEREL)

# Setting CXX Flag /MD or /MT and POSTFIX values i.e MDd / MD / MTd / MT / d
# using CMake variable CMAKE_MSVC_RUNTIME_LIBRARY.
#
# For visual studio the library naming is as following:
#  Dynamic libraries:
#   - PocoX.dll  for release library
#   - PocoXd.dll for debug library
#
#  Static libraries:
#   - PocoXmd.lib for /MD release build
#   - PocoXtmt.lib for /MT release build
#
#   - PocoXmdd.lib for /MD debug build
#   - PocoXmtd.lib for /MT debug build

if(BUILD_SHARED_LIBS)
	add_compile_definitions(POCO_DLL)
else()
	add_compile_definitions(POCO_STATIC)
endif()

if(MSVC)
	if(POCO_MT)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
		set(STATIC_POSTFIX "mt" CACHE STRING "Set static library postfix" FORCE)
	else(POCO_MT)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
		set(STATIC_POSTFIX "md" CACHE STRING "Set static library postfix" FORCE)
	endif(POCO_MT)

	message(STATUS "MSVC runtime library: ${CMAKE_MSVC_RUNTIME_LIBRARY}")

	if(POCO_SANITIZE_ASAN)
		message(WARNING "Use POCO_SANITIZEFLAGS instead of POCO_SANITIZE_ASAN")
		add_compile_options("/fsanitize=address")
	endif()

else(MSVC)
	# Other compilers then MSVC don't have a static STATIC_POSTFIX at the moment
	set(STATIC_POSTFIX "" CACHE STRING "Set static library postfix" FORCE)
endif(MSVC)

if (DEFINED POCO_SANITIZEFLAGS AND NOT "${POCO_SANITIZEFLAGS}" STREQUAL "")
	message(STATUS "Using sanitize flags: ${POCO_SANITIZEFLAGS}")
	add_compile_options(${POCO_SANITIZEFLAGS})
	add_link_options(${POCO_SANITIZEFLAGS})
endif()

if (ENABLE_COMPILER_WARNINGS)
	message(STATUS "Enabling additional compiler warning flags.")
	# Additional compiler-specific warning flags
	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		# using clang
		add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		# using GCC
		add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		# using Visual Studio C++
		add_compile_options(/W4)
	endif()
endif()

# Add a d postfix to the debug libraries
if(BUILD_SHARED_LIBS)
	set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Set Debug library postfix" FORCE)
	set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "Set Release library postfix" FORCE)
	set(CMAKE_MINSIZEREL_POSTFIX "" CACHE STRING "Set MinSizeRel library postfix" FORCE)
	set(CMAKE_RELWITHDEBINFO_POSTFIX "" CACHE STRING "Set RelWithDebInfo library postfix" FORCE)
else(BUILD_SHARED_LIBS)
	set(CMAKE_DEBUG_POSTFIX "${STATIC_POSTFIX}d" CACHE STRING "Set Debug library postfix" FORCE)
	set(CMAKE_RELEASE_POSTFIX "${STATIC_POSTFIX}" CACHE STRING "Set Release library postfix" FORCE)
	set(CMAKE_MINSIZEREL_POSTFIX "${STATIC_POSTFIX}" CACHE STRING "Set MinSizeRel library postfix" FORCE)
	set(CMAKE_RELWITHDEBINFO_POSTFIX "${STATIC_POSTFIX}" CACHE STRING "Set RelWithDebInfo library postfix" FORCE)
endif()

# MacOS version that has full support for C++17
set(CMAKE_OSX_DEPLOYMENT_TARGET, 10.15)

# OS Detection
include(CheckTypeSize)
find_package(Cygwin)
