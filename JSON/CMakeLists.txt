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

add_library(JSON ${SRCS})
add_library(Poco::JSON ALIAS JSON)

target_link_libraries(JSON PRIVATE "$<BUILD_LOCAL_INTERFACE:PDJson::PDJson>")

set_target_properties(JSON
	PROPERTIES
		VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
		OUTPUT_NAME PocoJSON
		DEFINE_SYMBOL JSON_EXPORTS
)

target_link_libraries(JSON PUBLIC Poco::Foundation)
target_include_directories(JSON
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(JSON)
POCO_GENERATE_PACKAGE(JSON)

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()

if(ENABLE_FUZZING)
	add_subdirectory(fuzzing)
endif()
