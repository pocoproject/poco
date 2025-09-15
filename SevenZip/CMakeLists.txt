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

add_library(SevenZip ${SRCS})
add_library(Poco::SevenZip ALIAS SevenZip)

target_link_libraries(SevenZip PRIVATE "$<BUILD_LOCAL_INTERFACE:7Zip::7Zip>")

set_target_properties(SevenZip
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoSevenZip
	DEFINE_SYMBOL SevenZip_EXPORTS
)

target_link_libraries(SevenZip PUBLIC Poco::Foundation)

target_include_directories(SevenZip
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(SevenZip)
POCO_GENERATE_PACKAGE(SevenZip)

if(ENABLE_TESTS)
	add_subdirectory(samples)
endif()

