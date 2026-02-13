# PocoCompilerSelect.cmake
#
# Provides POCO_COMPILER and POCO_CROSS_COMPILE cache variables for
# convenient compiler and cross-compilation target selection.
#
# This file MUST be included before project() because CMAKE_C_COMPILER
# and CMAKE_CXX_COMPILER have no effect after the project() call.
#
# Usage examples:
#   cmake -B build -DPOCO_COMPILER=gcc
#   cmake -B build -DPOCO_COMPILER=gcc-12
#   cmake -B build -DPOCO_COMPILER=clang-18
#   cmake -B build -DPOCO_COMPILER=clang-cl
#   cmake -B build -DPOCO_COMPILER=/opt/gcc-13/bin/g++-13
#   cmake -B build -DPOCO_CROSS_COMPILE=arm-linux
#   cmake -B build -DPOCO_CROSS_COMPILE=android -DANDROID_ABI=armeabi-v7a
#   cmake -B build -DPOCO_CROSS_COMPILE=emscripten

set(POCO_COMPILER "" CACHE STRING
	"Select compiler: 'gcc', 'gcc-12', 'clang', 'clang-18', 'clang-cl', or an absolute path to a C++ compiler")

set(POCO_CROSS_COMPILE "" CACHE STRING
	"Cross-compilation target: 'arm-linux', 'android', or 'emscripten'")

# Guards (only relevant when POCO options are used)
if(NOT POCO_COMPILER STREQUAL "" AND NOT POCO_CROSS_COMPILE STREQUAL "")
	message(FATAL_ERROR
		"POCO_COMPILER and POCO_CROSS_COMPILE cannot be used together. "
		"Cross-compilation targets set their own compilers.")
endif()

if(NOT POCO_CROSS_COMPILE STREQUAL "" AND DEFINED CMAKE_TOOLCHAIN_FILE)
	message(FATAL_ERROR
		"POCO_CROSS_COMPILE cannot be used with -DCMAKE_TOOLCHAIN_FILE. "
		"The cross-compile option sets its own toolchain file internally.")
endif()

#################################################################################
# POCO_COMPILER: compiler selection
#################################################################################
if(NOT POCO_COMPILER STREQUAL "")

	if(IS_ABSOLUTE "${POCO_COMPILER}")
		# Absolute path to a C++ compiler executable
		if(NOT EXISTS "${POCO_COMPILER}")
			message(FATAL_ERROR
				"POCO_COMPILER path does not exist: '${POCO_COMPILER}'")
		endif()

		set(CMAKE_CXX_COMPILER "${POCO_COMPILER}")

		# Try to derive C compiler from C++ compiler path
		string(REPLACE "g++" "gcc" _poco_c_compiler "${POCO_COMPILER}")
		string(REPLACE "clang++" "clang" _poco_c_compiler "${_poco_c_compiler}")
		string(REPLACE "c++" "cc" _poco_c_compiler "${_poco_c_compiler}")

		if(EXISTS "${_poco_c_compiler}" AND NOT "${_poco_c_compiler}" STREQUAL "${POCO_COMPILER}")
			set(CMAKE_C_COMPILER "${_poco_c_compiler}")
			message(STATUS "[poco] POCO_COMPILER: CXX='${POCO_COMPILER}', C='${_poco_c_compiler}' (derived)")
		else()
			message(STATUS "[poco] POCO_COMPILER: CXX='${POCO_COMPILER}' (C compiler not derived, CMake will detect)")
		endif()

	elseif(POCO_COMPILER MATCHES "^gcc(-[0-9]+)?$")
		# GCC: "gcc" or "gcc-12"
		string(REGEX MATCH "-[0-9]+" _version_suffix "${POCO_COMPILER}")
		set(CMAKE_C_COMPILER "gcc${_version_suffix}")
		set(CMAKE_CXX_COMPILER "g++${_version_suffix}")
		message(STATUS "[poco] POCO_COMPILER: Using GCC (CC='${CMAKE_C_COMPILER}', CXX='${CMAKE_CXX_COMPILER}')")

	elseif(POCO_COMPILER MATCHES "^clang(-[0-9]+)?$")
		# Clang: "clang" or "clang-18"
		string(REGEX MATCH "-[0-9]+" _version_suffix "${POCO_COMPILER}")
		set(CMAKE_C_COMPILER "clang${_version_suffix}")
		set(CMAKE_CXX_COMPILER "clang++${_version_suffix}")
		message(STATUS "[poco] POCO_COMPILER: Using Clang (CC='${CMAKE_C_COMPILER}', CXX='${CMAKE_CXX_COMPILER}')")

	elseif(POCO_COMPILER STREQUAL "clang-cl")
		# Clang-CL: MSVC-compatible Clang on Windows
		set(CMAKE_C_COMPILER "clang-cl")
		set(CMAKE_CXX_COMPILER "clang-cl")
		message(STATUS "[poco] POCO_COMPILER: Using clang-cl")

	else()
		message(FATAL_ERROR
			"POCO_COMPILER='${POCO_COMPILER}' is not recognized.\n"
			"Expected: 'gcc', 'gcc-<version>', 'clang', 'clang-<version>', 'clang-cl', "
			"or an absolute path to a C++ compiler executable.")
	endif()

	set(POCO_COMPILER_SELECTION "${POCO_COMPILER}" CACHE INTERNAL "")
endif()

#################################################################################
# POCO_CROSS_COMPILE: cross-compilation targets
#################################################################################
if(NOT POCO_CROSS_COMPILE STREQUAL "")

	if(POCO_CROSS_COMPILE STREQUAL "arm-linux")
		set(CMAKE_SYSTEM_NAME "Linux")
		set(CMAKE_SYSTEM_PROCESSOR "aarch64")
		set(POCO_CROSS_COMPILE_TOOLCHAIN_PREFIX "aarch64-linux-gnu" CACHE STRING
			"Toolchain prefix for ARM Linux cross-compilation")
		set(CMAKE_C_COMPILER "${POCO_CROSS_COMPILE_TOOLCHAIN_PREFIX}-gcc")
		set(CMAKE_CXX_COMPILER "${POCO_CROSS_COMPILE_TOOLCHAIN_PREFIX}-g++")
		message(STATUS "[poco] Cross-compiling for ARM Linux (${POCO_CROSS_COMPILE_TOOLCHAIN_PREFIX})")

	elseif(POCO_CROSS_COMPILE STREQUAL "android")
		# Locate Android NDK
		if(DEFINED ENV{ANDROID_NDK_HOME})
			set(_ndk_path "$ENV{ANDROID_NDK_HOME}")
		elseif(DEFINED ENV{ANDROID_NDK})
			set(_ndk_path "$ENV{ANDROID_NDK}")
		elseif(DEFINED ENV{ANDROID_HOME})
			file(GLOB _ndk_candidates "$ENV{ANDROID_HOME}/ndk/*")
			list(LENGTH _ndk_candidates _ndk_count)
			if(_ndk_count GREATER 0)
				list(SORT _ndk_candidates ORDER DESCENDING)
				list(GET _ndk_candidates 0 _ndk_path)
			endif()
		endif()

		if(NOT DEFINED _ndk_path OR NOT EXISTS "${_ndk_path}")
			message(FATAL_ERROR
				"POCO_CROSS_COMPILE='android' requires ANDROID_NDK_HOME or ANDROID_NDK "
				"environment variable pointing to the Android NDK installation.")
		endif()

		set(_ndk_toolchain "${_ndk_path}/build/cmake/android.toolchain.cmake")
		if(NOT EXISTS "${_ndk_toolchain}")
			message(FATAL_ERROR
				"Android NDK toolchain file not found at: ${_ndk_toolchain}")
		endif()

		set(CMAKE_TOOLCHAIN_FILE "${_ndk_toolchain}")
		if(NOT DEFINED ANDROID_ABI)
			set(ANDROID_ABI "arm64-v8a" CACHE STRING "Android ABI")
		endif()
		if(NOT DEFINED ANDROID_PLATFORM)
			set(ANDROID_PLATFORM "android-24" CACHE STRING "Android platform level")
		endif()
		message(STATUS "[poco] Cross-compiling for Android (NDK: ${_ndk_path}, ABI: ${ANDROID_ABI})")

	elseif(POCO_CROSS_COMPILE STREQUAL "emscripten")
		if(DEFINED ENV{EMSDK})
			set(_emsdk_path "$ENV{EMSDK}")
		else()
			message(FATAL_ERROR
				"POCO_CROSS_COMPILE='emscripten' requires the EMSDK environment variable. "
				"Run 'source emsdk_env.sh' before configuring.")
		endif()

		set(_em_toolchain "${_emsdk_path}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
		if(NOT EXISTS "${_em_toolchain}")
			message(FATAL_ERROR
				"Emscripten toolchain file not found at: ${_em_toolchain}")
		endif()

		set(CMAKE_TOOLCHAIN_FILE "${_em_toolchain}")
		message(STATUS "[poco] Cross-compiling for Emscripten/WebAssembly (EMSDK: ${_emsdk_path})")

	else()
		message(FATAL_ERROR
			"POCO_CROSS_COMPILE='${POCO_CROSS_COMPILE}' is not recognized.\n"
			"Supported targets: 'arm-linux', 'android', 'emscripten'")
	endif()

	set(POCO_COMPILER_SELECTION "cross: ${POCO_CROSS_COMPILE}" CACHE INTERNAL "")
endif()

#################################################################################
# Auto-detect host architecture for clang-cl on Windows
#
# clang-cl defaults to x86 regardless of host. This detects the host
# architecture via the Windows PROCESSOR_ARCHITECTURE environment variable
# and sets the appropriate --target triple so builds match the host.
# Runs regardless of whether POCO_COMPILER was used, so it also works
# with -DCMAKE_CXX_COMPILER=clang-cl.
#################################################################################
if(DEFINED ENV{PROCESSOR_ARCHITECTURE})
	set(_poco_compiler_to_check "${CMAKE_CXX_COMPILER}")
	if(_poco_compiler_to_check MATCHES "[Cc]lang-[Cc][Ll]")
		# Only attempt to auto-set targets if at least one is not already defined,
		# so we don't override fully user-defined configuration.
		if(NOT DEFINED CMAKE_C_COMPILER_TARGET OR NOT DEFINED CMAKE_CXX_COMPILER_TARGET)
			set(_poco_auto_target "")
			if("$ENV{PROCESSOR_ARCHITECTURE}" STREQUAL "ARM64")
				set(_poco_auto_target "arm64-pc-windows-msvc")
			elseif("$ENV{PROCESSOR_ARCHITECTURE}" STREQUAL "AMD64")
				set(_poco_auto_target "x86_64-pc-windows-msvc")
			endif()
			if(NOT _poco_auto_target STREQUAL "")
				if(NOT DEFINED CMAKE_C_COMPILER_TARGET)
					set(CMAKE_C_COMPILER_TARGET "${_poco_auto_target}")
				endif()
				if(NOT DEFINED CMAKE_CXX_COMPILER_TARGET)
					set(CMAKE_CXX_COMPILER_TARGET "${_poco_auto_target}")
				endif()
				message(STATUS "[poco] Auto-detected clang-cl target: ${_poco_auto_target}")
			endif()
			unset(_poco_auto_target)
		endif()
	endif()
	unset(_poco_compiler_to_check)
endif()
