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

add_library(Zip ${SRCS})
if (POCO_UNBUNDLED)
	target_link_libraries(Zip PRIVATE ZLIB::ZLIB)
	target_compile_definitions(Zip PUBLIC POCO_UNBUNDLED)
else()
	target_link_libraries(Zip PRIVATE "$<BUILD_LOCAL_INTERFACE:ZLIB::ZLIB>")
endif()

add_library(Poco::Zip ALIAS Zip)
set_target_properties(Zip
	PROPERTIES
		VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
		OUTPUT_NAME PocoZip
		DEFINE_SYMBOL Zip_EXPORTS
)

target_link_libraries(Zip PUBLIC Poco::Foundation)
target_include_directories(Zip
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(Zip)
POCO_GENERATE_PACKAGE(Zip)

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()

