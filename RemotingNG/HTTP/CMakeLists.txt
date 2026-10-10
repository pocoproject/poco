# Sources
file(GLOB SRCS_HTTP_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS_HTTP ${SRCS_HTTP_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS_HTTP ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS_HTTP ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(RemotingNGHTTP ${SRCS_HTTP})
add_library(Poco::RemotingNGHTTP ALIAS RemotingNGHTTP)
set_target_properties(RemotingNGHTTP
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoRemotingNGHTTP
	DEFINE_SYMBOL RemotingNGHTTP_EXPORTS
)

target_link_libraries(RemotingNGHTTP PUBLIC Poco::Foundation Poco::Net Poco::RemotingNG)
target_include_directories(RemotingNGHTTP
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(RemotingNGHTTP)
POCO_GENERATE_PACKAGE(RemotingNGHTTP)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
