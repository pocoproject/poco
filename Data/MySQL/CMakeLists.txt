# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( MYSQL_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( MYSQL_SRCS ${HDRS_G})

add_library(DataMySQL ${MYSQL_SRCS} )
add_library(Poco::DataMySQL ALIAS DataMySQL)
set_target_properties(DataMySQL
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME PocoDataMySQL
    DEFINE_SYMBOL MySQL_EXPORTS
    )

target_link_libraries(DataMySQL PUBLIC Poco::Data ${MYSQL_LIB})
target_include_directories(DataMySQL
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions(DataMySQL PUBLIC THREADSAFE NO_TCL)

POCO_INSTALL(DataMySQL)
POCO_GENERATE_PACKAGE(DataMySQL)

if (ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
