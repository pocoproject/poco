# Sources
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (WIN32)

file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})
POCO_SOURCES(SRCS pdjson src/pdjson.c)
if(WIN32)
    SET_SOURCE_FILES_PROPERTIES(src/pdjson.c PROPERTIES LANGUAGE CXX)
endif()

add_library(JSON ${LIB_MODE} ${SRCS} )
add_library(Poco::JSON ALIAS JSON)
set_target_properties(JSON
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoJSON
    DEFINE_SYMBOL JSON_EXPORTS
    )

target_link_libraries(JSON Poco::Foundation)
target_include_directories(JSON
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions(JSON PUBLIC ${LIB_MODE_DEFINITIONS})

POCO_INSTALL(JSON)
POCO_GENERATE_PACKAGE(JSON)

if (ENABLE_TESTS)
    add_subdirectory(samples)
    add_subdirectory(testsuite)
endif ()

