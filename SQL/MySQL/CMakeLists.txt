# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( MYSQL_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( MYSQL_SRCS ${HDRS_G})


add_library(SQLMySQL ${MYSQL_SRCS} )
add_library(Poco::SQLMySQL ALIAS SQLMySQL)
set_target_properties(SQLMySQL
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoSQLMySQL
    DEFINE_SYMBOL MySQL_EXPORTS
    )

target_link_libraries(SQLMySQL PUBLIC Poco::SQL ${MYSQL_LIB}) # TODO(Bjoe) Use namespace target
target_include_directories(SQLMySQL
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions(SQLMySQL PUBLIC THREADSAFE NO_TCL)

if(MINGW) # MinGW supports 'mariadb-connector-c' only with OpenSSL
	target_link_libraries(SQLMySQL PUBLIC ws2_32 ${OPENSSL_LIBRARIES})
endif()

POCO_INSTALL(SQLMySQL)
POCO_GENERATE_PACKAGE(SQLMySQL)

if (POCO_ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()

configure_file(PocoSQLMySQL.pc.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoSQLMySQL.pc @ONLY)
list(APPEND POCO_PKG_CONFIG_FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoSQLMySQL.pc)
set(POCO_PKG_CONFIG_FILES ${POCO_PKG_CONFIG_FILES} PARENT_SCOPE)
