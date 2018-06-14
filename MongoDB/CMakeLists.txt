# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

add_library(MongoDB ${SRCS} )
add_library(Poco::MongoDB ALIAS MongoDB)
set_target_properties(MongoDB
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoMongoDB
    DEFINE_SYMBOL MongoDB_EXPORTS
    )

target_link_libraries(MongoDB PUBLIC Poco::Net)
target_include_directories(MongoDB
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

POCO_INSTALL(MongoDB)
POCO_GENERATE_PACKAGE(MongoDB)

if (POCO_ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
if (POCO_ENABLE_SAMPLES)
    add_subdirectory(samples)
endif ()

configure_file(PocoMongoDB.pc.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoMongoDB.pc @ONLY)
list(APPEND POCO_PKG_CONFIG_FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoMongoDB.pc)
set(POCO_PKG_CONFIG_FILES ${POCO_PKG_CONFIG_FILES} PARENT_SCOPE)
