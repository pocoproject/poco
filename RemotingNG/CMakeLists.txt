# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(RemotingNG ${SRCS})
add_library(Poco::RemotingNG ALIAS RemotingNG)
set_target_properties(RemotingNG
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoRemotingNG
	DEFINE_SYMBOL RemotingNG_EXPORTS
)

target_link_libraries(RemotingNG PUBLIC Poco::Foundation)
target_include_directories(RemotingNG
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(RemotingNG)
POCO_GENERATE_PACKAGE(RemotingNG)

if(ENABLE_REMOTINGNG_TCP)
	add_subdirectory(TCP)
endif()

if(ENABLE_REMOTINGNG_HTTP)
	add_subdirectory(HTTP)
endif()

if(ENABLE_REMOTINGNG_JSONRPC)
	add_subdirectory(JSONRPC)
endif()

if(ENABLE_REMOTINGNG_REST)
	add_subdirectory(REST)
endif()

if(ENABLE_REMOTINGNG_SOAP)
	add_subdirectory(SOAP)
endif()

if(ENABLE_REMOTINGNG_REMOTEGEN)
	add_subdirectory(RemoteGen)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()
