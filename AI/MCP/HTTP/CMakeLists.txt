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

add_library(AIMCPHTTP ${SRCS})
add_library(Poco::AIMCPHTTP ALIAS AIMCPHTTP)
set_target_properties(AIMCPHTTP
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoAIMCPHTTP
	DEFINE_SYMBOL AIMCPHTTP_EXPORTS
)

# Net only: HTTPS comes from Poco::Net::HTTPSessionFactory and the
# instantiator the application registers, never from a NetSSL link.
target_link_libraries(AIMCPHTTP PUBLIC Poco::AIMCP Poco::Net)
target_include_directories(AIMCPHTTP
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(AIMCPHTTP)
POCO_GENERATE_PACKAGE(AIMCPHTTP)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
