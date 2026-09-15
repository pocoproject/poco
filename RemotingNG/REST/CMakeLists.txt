# Sources
file(GLOB SRCS_REST_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS_REST ${SRCS_REST_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS_REST ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS_REST ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(RemotingNGREST ${SRCS_REST})
add_library(Poco::RemotingNGREST ALIAS RemotingNGREST)
set_target_properties(RemotingNGREST
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoRemotingNGREST
	DEFINE_SYMBOL RemotingNGREST_EXPORTS
)

target_link_libraries(RemotingNGREST
	PUBLIC
	Poco::Foundation Poco::Net Poco::RemotingNG
)

target_include_directories(RemotingNGREST
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(RemotingNGREST)
POCO_GENERATE_PACKAGE(RemotingNGREST)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
