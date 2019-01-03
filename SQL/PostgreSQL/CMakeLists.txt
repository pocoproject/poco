# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( POSTGRESQL_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( POSTGRESQL_SRCS ${HDRS_G})

add_library(SQLPostgreSQL ${POSTGRESQL_SRCS} )
add_library(Poco::SQLPostgreSQL ALIAS SQLPostgreSQL)
set_target_properties(SQLPostgreSQL
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoSQLPostgreSQL
    DEFINE_SYMBOL PostgreSQL_EXPORTS
    )

target_link_libraries(SQLPostgreSQL PUBLIC Poco::SQL PostgreSQL::client)
target_include_directories(SQLPostgreSQL
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

POCO_INSTALL(SQLPostgreSQL)
POCO_GENERATE_PACKAGE(SQLPostgreSQL)

if (POCO_ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()

configure_file(PocoSQLPostgreSQL.pc.in ${CMAKE_BINARY_DIR}/PocoSQLPostgreSQL.pc @ONLY)
list(APPEND POCO_PKG_CONFIG_FILES ${CMAKE_BINARY_DIR}/PocoSQLPostgreSQL.pc)
set(POCO_PKG_CONFIG_FILES ${POCO_PKG_CONFIG_FILES} PARENT_SCOPE)
