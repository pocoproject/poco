# Expat CPP sources to be excluded from globbed SRCS_G
# They are added back on the list if POCO_UNBUNDLED is not set
set(EXPAT_CPP "${CMAKE_CURRENT_SOURCE_DIR}/src/xmlparse.cpp")

# Sources
file(GLOB SRCS_G "src/*.cpp")
list(REMOVE_ITEM SRCS_G ${EXPAT_CPP})
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

# If POCO_UNBUNDLED is enabled we try to find the required packages
# The configuration will fail if the packages are not found
if(POCO_UNBUNDLED)
	find_package(EXPAT REQUIRED)
else()
	POCO_SOURCES(SRCS expat
		src/xmlparse.cpp
		src/xmlrole.c
		src/xmltok.c
		src/xmltok_impl.c
		src/xmltok_ns.c
	)
endif(POCO_UNBUNDLED)

add_library(XML ${SRCS})

add_library(Poco::XML ALIAS XML)
set_target_properties(XML
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoXML
	DEFINE_SYMBOL XML_EXPORTS
)

target_link_libraries(XML PUBLIC Poco::Foundation)
target_include_directories(XML
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(POCO_UNBUNDLED)
	target_link_libraries(XML PUBLIC EXPAT::EXPAT)
	target_compile_definitions(XML PUBLIC POCO_UNBUNDLED)
else()
	if(WIN32)
		#TODO: Is XML_STATIC only required with Windows? What does it do?
		target_compile_definitions(XML PUBLIC XML_STATIC)
	endif()
	target_compile_definitions(XML
		PUBLIC XML_DTD XML_GE
		PRIVATE XML_NS HAVE_EXPAT_CONFIG_H)
endif()

POCO_INSTALL(XML)
POCO_GENERATE_PACKAGE(XML)

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()

