# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(MCP_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(MCP_SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND MCP_SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(AIMCP ${MCP_SRCS})
add_library(Poco::AIMCP ALIAS AIMCP)
set_target_properties(AIMCP
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoAIMCP
	DEFINE_SYMBOL AIMCP_EXPORTS
)

# The core is the protocol plus the stdio transport: JSON and Foundation only.
target_link_libraries(AIMCP PUBLIC Poco::JSON)
target_include_directories(AIMCP
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(AIMCP)
POCO_GENERATE_PACKAGE(AIMCP)

if(ENABLE_AI_MCP_HTTP)
	add_subdirectory(HTTP)
endif()

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
