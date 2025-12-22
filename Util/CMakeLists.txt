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

POCO_SOURCES_AUTO_PLAT(SRCS WIN32
	src/WinRegistryConfiguration.cpp
	src/WinRegistryKey.cpp
	src/WinService.cpp
)

add_library(Util ${SRCS})
add_library(Poco::Util ALIAS Util)
set_target_properties(Util
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoUtil
	DEFINE_SYMBOL Util_EXPORTS
)

target_link_libraries(Util PUBLIC Poco::Foundation)
if(ENABLE_XML)
	target_link_libraries(Util PUBLIC Poco::XML)
else()
	target_compile_definitions(Util PUBLIC POCO_UTIL_NO_XMLCONFIGURATION)
endif()
if(ENABLE_JSON)
	target_link_libraries(Util PUBLIC Poco::JSON)
else()
	target_compile_definitions(Util PUBLIC POCO_UTIL_NO_JSONCONFIGURATION)
endif()

target_include_directories(Util
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(Util)
POCO_GENERATE_PACKAGE(Util)

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
