# cpptrace - C++ stack trace library
# Version: 1.0.4
# Source: https://github.com/jeremy-rifkin/cpptrace/releases/tag/v1.0.4
#
# cpptrace is always bundled (no POCO_UNBUNDLED support).

# Sources
file(GLOB_RECURSE SRCS_G "src/*.cpp")
POCO_SOURCES(SRCS cpptrace ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.hpp" "include/*.h" "src/*.hpp" "src/*.h")
POCO_HEADERS(SRCS cpptrace ${HDRS_G})

add_library(_BUNDLED_CPPTRACE OBJECT EXCLUDE_FROM_ALL ${SRCS})

target_compile_definitions(_BUNDLED_CPPTRACE PUBLIC CPPTRACE_STATIC_DEFINE)

set_property(TARGET _BUNDLED_CPPTRACE PROPERTY POSITION_INDEPENDENT_CODE ON)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		# Find backtrace header provided by gcc with a package like "libgcc-11-dev"
		file(GLOB_RECURSE BACKTRACE_H /usr/lib/gcc/*backtrace.h)
		if(NOT BACKTRACE_H)
			message(FATAL_ERROR "cpptrace: backtrace.h not found. GCC development package not installed.")
		endif()
		list(GET BACKTRACE_H 0 FIRST_BACKTRACE)
		cmake_path(GET FIRST_BACKTRACE PARENT_PATH BACKTRACE_INCLUDE)
		message(STATUS "Clang on Linux uses libbacktrace provided by gcc. backtrace.h found in ${BACKTRACE_INCLUDE}.")
		target_include_directories(_BUNDLED_CPPTRACE PRIVATE ${BACKTRACE_INCLUDE})
	endif()

	target_compile_definitions(_BUNDLED_CPPTRACE PRIVATE CPPTRACE_DEMANGLE_WITH_CXXABI CPPTRACE_UNWIND_WITH_UNWIND)
	target_compile_definitions(_BUNDLED_CPPTRACE PRIVATE CPPTRACE_GET_SYMBOLS_WITH_LIBBACKTRACE)
	target_link_libraries(_BUNDLED_CPPTRACE PRIVATE backtrace)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
	find_library(BACKTRACE_LIBRARY backtrace REQUIRED)
	cmake_path(GET BACKTRACE_LIBRARY PARENT_PATH BACKTRACE_LIBDIR)
	set(BACKTRACE_INCLUDE_DIR "${BACKTRACE_LIBDIR}/../include")
	cmake_path(ABSOLUTE_PATH BACKTRACE_INCLUDE_DIR NORMALIZE)
	find_file(BACKTRACE_H backtrace.h PATHS ${BACKTRACE_INCLUDE_DIR} REQUIRED)
	target_compile_definitions(_BUNDLED_CPPTRACE PRIVATE CPPTRACE_DEMANGLE_WITH_CXXABI CPPTRACE_UNWIND_WITH_UNWIND)
	target_compile_definitions(_BUNDLED_CPPTRACE PRIVATE CPPTRACE_GET_SYMBOLS_WITH_LIBBACKTRACE)
	target_include_directories(_BUNDLED_CPPTRACE PRIVATE ${BACKTRACE_INCLUDE_DIR})
	target_link_libraries(_BUNDLED_CPPTRACE PRIVATE ${BACKTRACE_LIBRARY})
elseif(APPLE)
	target_compile_definitions(_BUNDLED_CPPTRACE PRIVATE CPPTRACE_DEMANGLE_WITH_CXXABI CPPTRACE_UNWIND_WITH_UNWIND)
	target_compile_definitions(_BUNDLED_CPPTRACE PRIVATE CPPTRACE_GET_SYMBOLS_WITH_LIBDL)
elseif(WIN32)
	target_compile_definitions(_BUNDLED_CPPTRACE PRIVATE CPPTRACE_DEMANGLE_WITH_WINAPI CPPTRACE_UNWIND_WITH_WINAPI)
	target_compile_definitions(_BUNDLED_CPPTRACE PRIVATE CPPTRACE_GET_SYMBOLS_WITH_DBGHELP NOMINMAX)
	target_link_libraries(_BUNDLED_CPPTRACE PRIVATE dbghelp)
else()
	message(FATAL_ERROR "cpptrace: Unsupported platform.")
endif()

target_include_directories(_BUNDLED_CPPTRACE
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(cpptrace::cpptrace ALIAS _BUNDLED_CPPTRACE)
