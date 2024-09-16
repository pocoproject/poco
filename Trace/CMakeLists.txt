# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h*")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(Trace ${SRCS})
add_library(Poco::Trace ALIAS Trace)
set_target_properties(Trace
	PROPERTIES
		VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
		OUTPUT_NAME PocoTrace
		DEFINE_SYMBOL Trace_EXPORTS
)

target_compile_definitions (Trace PRIVATE cpptrace_lib_EXPORTS)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	target_compile_definitions (Trace PRIVATE CPPTRACE_DEMANGLE_WITH_CXXABI CPPTRACE_UNWIND_WITH_UNWIND)
	target_compile_definitions (Trace PRIVATE CPPTRACE_GET_SYMBOLS_WITH_LIBBACKTRACE)
	target_link_libraries(Trace PRIVATE backtrace)
elseif (APPLE)
	target_compile_definitions (Trace PRIVATE CPPTRACE_DEMANGLE_WITH_CXXABI CPPTRACE_UNWIND_WITH_UNWIND)
	target_compile_definitions (Trace PRIVATE CPPTRACE_GET_SYMBOLS_WITH_LIBDL)
elseif(WIN32)
	target_compile_definitions (Trace PRIVATE CPPTRACE_DEMANGLE_WITH_WINAPI CPPTRACE_UNWIND_WITH_WINAPI)
	target_compile_definitions (Trace PRIVATE CPPTRACE_GET_SYMBOLS_WITH_DBGHELP NOMINMAX)
	target_link_libraries(Trace PRIVATE dbghelp)
else()
	message(FATAL_ERROR "Poco::Trace: Unsupported platform.")
endif()

target_compile_definitions (Trace PUBLIC POCO_ENABLE_TRACE)

target_include_directories(Trace
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Poco/Trace>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(Trace)
POCO_GENERATE_PACKAGE(Trace)
