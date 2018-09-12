# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})
POCO_SOURCES(SRCS pdjson src/pdjson.c)

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

add_library(JSON ${SRCS} )
add_library(Poco::JSON ALIAS JSON)
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
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

if(MSVC)
	set_property(TARGET JSON PROPERTY C_STANDARD 99)
else()
	target_compile_features(JSON PUBLIC c_variadic_macros)
endif()

POCO_INSTALL(JSON)
POCO_GENERATE_PACKAGE(JSON)

if (POCO_ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
if (POCO_ENABLE_SAMPLES)
    add_subdirectory(samples)
endif ()

configure_file(PocoJSON.pc.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoJSON.pc @ONLY)
list(APPEND POCO_PKG_CONFIG_FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoJSON.pc)
set(POCO_PKG_CONFIG_FILES ${POCO_PKG_CONFIG_FILES} PARENT_SCOPE)
