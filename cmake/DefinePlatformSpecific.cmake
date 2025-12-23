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

	# Strip debug symbols from Release binaries (reduces binary size significantly)
	# On Windows, debug symbols go to separate .pdb files, so this is not needed
	# NOTE: CMAKE_BUILD_TYPE must be set to Release for this to take effect
	#       e.g., cmake -B build -DCMAKE_BUILD_TYPE=Release
	add_link_options($<$<CONFIG:Release>:-s>)
endif(MSVC)

if (DEFINED POCO_SANITIZEFLAGS AND NOT "${POCO_SANITIZEFLAGS}" STREQUAL "")
	message(STATUS "Using sanitize flags: ${POCO_SANITIZEFLAGS}")
	add_compile_options(${POCO_SANITIZEFLAGS})
	add_link_options(${POCO_SANITIZEFLAGS})
endif()

#################################################################################
# Compiler warnings for Poco code only
#################################################################################
# This function enables additional compiler warnings for Poco C++ code.
# It should be called from the root CMakeLists.txt AFTER add_subdirectory(dependencies)
# to ensure third-party code is not affected.
#
# The function uses $<COMPILE_LANGUAGE:CXX> generator expressions to apply
# warnings only to C++ files, providing an extra layer of protection since
# bundled dependencies are mostly C code.
#
function(poco_enable_detailed_compiler_warnings)
	if (NOT ENABLE_COMPILER_WARNINGS)
		return()
	endif()

	message(STATUS "Enabling additional compiler warning flags for Poco C++ code only.")

	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		# Clang and AppleClang
		add_compile_options(
			$<$<COMPILE_LANGUAGE:CXX>:-Wall>
			$<$<COMPILE_LANGUAGE:CXX>:-Wextra>
			$<$<COMPILE_LANGUAGE:CXX>:-Wpedantic>
			$<$<COMPILE_LANGUAGE:CXX>:-Wno-unused-parameter>
			$<$<COMPILE_LANGUAGE:CXX>:-Wzero-as-null-pointer-constant>
		)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		# GCC
		add_compile_options(
			$<$<COMPILE_LANGUAGE:CXX>:-Wall>
			$<$<COMPILE_LANGUAGE:CXX>:-Wextra>
			$<$<COMPILE_LANGUAGE:CXX>:-Wpedantic>
			$<$<COMPILE_LANGUAGE:CXX>:-Wno-unused-parameter>
			$<$<COMPILE_LANGUAGE:CXX>:-Wzero-as-null-pointer-constant>
		)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		# Visual Studio
		add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/W4>)
	endif()
endfunction(poco_enable_detailed_compiler_warnings)

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

# OS Detection
include(CheckTypeSize)
find_package(Cygwin)
