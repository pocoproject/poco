set(LIBNAME "SQL")
set(POCO_LIBNAME "Poco${LIBNAME}")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

if (BUILD_SHARED_LIBS)
    add_definitions(-DTHREADSAFE)
endif (BUILD_SHARED_LIBS)

if(MSVC AND NOT(MSVC_VERSION LESS 1400))
    set_source_files_properties(src/StatementImpl.cpp
        PROPERTIES COMPILE_FLAGS "/bigobj")
endif()

if(MINGW)
    set_source_files_properties(src/StatementImpl.cpp
        PROPERTIES COMPILE_FLAGS "-Wa,-mbig-obj")
endif()


add_library( "${LIBNAME}" ${SRCS} )
add_library( "${POCO_LIBNAME}" ALIAS "${LIBNAME}")
set_target_properties( "${LIBNAME}"
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME ${POCO_LIBNAME}
    DEFINE_SYMBOL SQL_EXPORTS
    )

target_link_libraries( "${LIBNAME}" Foundation)
target_include_directories( "${LIBNAME}"
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions("${LIBNAME}" PUBLIC ${LIB_MODE_DEFINITIONS})

POCO_INSTALL("${LIBNAME}")
POCO_GENERATE_PACKAGE("${LIBNAME}")

if(POCO_ENABLE_SQL_SQLITE)
    # SQlite3 is built in any case
    add_subdirectory( SQLite )
endif(POCO_ENABLE_SQL_SQLITE)

if(POCO_ENABLE_SQL_MYSQL)
    find_package(MySQL)
    if(MYSQL_FOUND)
        include_directories("${MYSQL_INCLUDE_DIR}")
        if(POCO_VERBOSE_MESSAGES)
            message(STATUS "MySQL Support Enabled")
        endif()
        add_subdirectory( MySQL )
    elseif(POCO_VERBOSE_MESSAGES)
        message(STATUS "MySQL Support Disabled - no MySQL library")
    endif(MYSQL_FOUND)
endif(POCO_ENABLE_SQL_MYSQL)

if(POCO_ENABLE_SQL_POSTGRESQL)
    find_package(PostgreSQL)
    if(POSTGRESQL_FOUND)
        include_directories("${PostgreSQL_INCLUDE_DIR}")
        if(POCO_VERBOSE_MESSAGES)
            message(STATUS "PostgreSQL Support Enabled")
        endif()
        add_subdirectory( PostgreSQL )
    elseif(POCO_VERBOSE_MESSAGES)
        message(STATUS "PostgreSQL Support Disabled - no PostgreSQL library")
    endif(POSTGRESQL_FOUND)
endif(POCO_ENABLE_SQL_POSTGRESQL)

if(POCO_ENABLE_SQL_ODBC)
    find_package(ODBC)
    if(WIN32 AND NOT WINCE)
        set(ODBC_LIBRARIES "odbc32" "odbccp32")
        if(POCO_VERBOSE_MESSAGES)
            message(STATUS "Windows native ODBC Support Enabled")
        endif()
        add_subdirectory( ODBC )
    else(WIN32 AND NOT WINCE)
        if(ODBC_FOUND)
            include_directories("${ODBC_INCLUDE_DIRECTORIES}")
            if(POCO_VERBOSE_MESSAGES)
                message(STATUS "ODBC Support Enabled")
            endif()
            add_subdirectory( ODBC )
        elseif(POCO_VERBOSE_MESSAGES)
            message(STATUS "ODBC Support Disabled - no ODBC runtime")
        endif()
    endif(WIN32 AND NOT WINCE)
endif(POCO_ENABLE_SQL_ODBC)

if (POCO_ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
if (POCO_ENABLE_SAMPLES)
    add_subdirectory(samples)
endif ()
