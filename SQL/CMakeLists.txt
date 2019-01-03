# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

if(MSVC AND NOT(MSVC_VERSION LESS 1400))
    set_source_files_properties(src/StatementImpl.cpp
        PROPERTIES COMPILE_FLAGS "/bigobj")
endif()

if(MINGW)
    set_source_files_properties(src/StatementImpl.cpp
        PROPERTIES COMPILE_FLAGS "-Wa,-mbig-obj")
endif()

add_library(SQL ${SRCS} )
add_library(Poco::SQL ALIAS SQL)
set_target_properties(SQL
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoSQL
    DEFINE_SYMBOL SQL_EXPORTS
    )

target_link_libraries(SQL PUBLIC Poco::Foundation)
target_include_directories(SQL
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

POCO_INSTALL(SQL)
POCO_GENERATE_PACKAGE(SQL)

if(POCO_ENABLE_SQL_SQLITE)
    if(POCO_VERBOSE_MESSAGES)
        message(STATUS "SQLite Support Enabled")
    endif()
    # SQlite3 is built in any case
    add_subdirectory( SQLite )
elseif(POCO_VERBOSE_MESSAGES)
    message(STATUS "SQLite Support Disabled")
endif(POCO_ENABLE_SQL_SQLITE)

if(MYSQL_FOUND AND POCO_ENABLE_SQL_MYSQL)
    if(POCO_VERBOSE_MESSAGES)
        message(STATUS "MySQL Support Enabled")
    endif()
    add_subdirectory( MySQL )
elseif(POCO_VERBOSE_MESSAGES)
    message(STATUS "MySQL Support Disabled - no MySQL library")
endif()

if(POSTGRESQL_FOUND AND POCO_ENABLE_SQL_POSTGRESQL)
    if(POCO_VERBOSE_MESSAGES)
        message(STATUS "PostgreSQL Support Enabled")
    endif()
    add_subdirectory( PostgreSQL )
elseif(POCO_VERBOSE_MESSAGES)
    message(STATUS "PostgreSQL Support Disabled - no PostgreSQL library")
endif()

if(WIN32 AND NOT WINCE AND POCO_ENABLE_SQL_ODBC)
    if(POCO_VERBOSE_MESSAGES)
        message(STATUS "Windows native ODBC Support Enabled")
    endif()
    add_subdirectory( ODBC )
else()
    if(ODBC_FOUND AND POCO_ENABLE_SQL_ODBC)
        if(POCO_VERBOSE_MESSAGES)
            message(STATUS "ODBC Support Enabled")
        endif()
        add_subdirectory( ODBC )
    elseif(POCO_VERBOSE_MESSAGES)
        message(STATUS "ODBC Support Disabled - no ODBC runtime")
    endif()
endif()

if (POCO_ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
if (POCO_ENABLE_SAMPLES)
    add_subdirectory(samples)
endif ()

configure_file(PocoSQL.pc.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoSQL.pc @ONLY)
list(APPEND POCO_PKG_CONFIG_FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoSQL.pc)
set(POCO_PKG_CONFIG_FILES ${POCO_PKG_CONFIG_FILES} PARENT_SCOPE)
