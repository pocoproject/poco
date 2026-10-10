# Sources
file(GLOB SRCS_SOAP_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS_SOAP ${SRCS_SOAP_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS_SOAP ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS_SOAP ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(RemotingNGSOAP ${SRCS_SOAP})
add_library(Poco::RemotingNGSOAP ALIAS RemotingNGSOAP)
set_target_properties(RemotingNGSOAP
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoRemotingNGSOAP
	DEFINE_SYMBOL RemotingNGSOAP_EXPORTS
)

target_link_libraries(RemotingNGSOAP PUBLIC Poco::Foundation Poco::XML Poco::Net Poco::RemotingNG)
target_include_directories(RemotingNGSOAP
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(RemotingNGSOAP)
POCO_GENERATE_PACKAGE(RemotingNGSOAP)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
