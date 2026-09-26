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

add_library(XSDTypes ${SRCS})
add_library(Poco::XSDTypes ALIAS XSDTypes)
set_target_properties(XSDTypes
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoXSDTypes
	DEFINE_SYMBOL XSDTypes_EXPORTS
)

target_link_libraries(XSDTypes PUBLIC Poco::Foundation Poco::XML)
target_include_directories(XSDTypes
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(XSDTypes)
POCO_GENERATE_PACKAGE(XSDTypes)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
