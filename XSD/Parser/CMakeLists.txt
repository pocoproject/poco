# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/ProDLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/ProDLLVersion.rc)
endif()

add_library(XSDParser ${SRCS})
add_library(Poco::XSDParser ALIAS XSDParser)
set_target_properties(XSDParser
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoXSDParser
	DEFINE_SYMBOL XSDParser_EXPORTS
)

target_link_libraries(XSDParser PUBLIC Poco::Foundation Poco::XSDTypes)
target_include_directories(XSDParser
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(XSDParser)
POCO_GENERATE_PACKAGE(XSDParser)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
