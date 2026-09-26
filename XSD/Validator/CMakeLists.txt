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

add_library(XSDValidator ${SRCS})
add_library(Poco::XSDValidator ALIAS XSDValidator)
set_target_properties(XSDValidator
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoXSDValidator
	DEFINE_SYMBOL XSDValidator_EXPORTS
)

target_link_libraries(XSDValidator
	PUBLIC Poco::Foundation
	PRIVATE LibXml2::LibXml2
)
target_include_directories(XSDValidator
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(XSDValidator)
POCO_GENERATE_PACKAGE(XSDValidator)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
