# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})
POCO_SOURCES(SRCS pdjson src/pdjson.c)

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

target_link_libraries(Trace PUBLIC Poco::Foundation)
target_include_directories(Trace
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(WIN32)
	target_compile_definitions(Trace PRIVATE _CRT_SECURE_NO_WARNINGS)
	set_source_files_properties(src/pdjson.c PROPERTIES LANGUAGE CXX)
	set_property(TARGET Trace PROPERTY C_STANDARD 99)
else()
	target_compile_features(Trace PUBLIC c_variadic_macros)
endif()

POCO_INSTALL(Trace)
POCO_GENERATE_PACKAGE(Trace)

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
