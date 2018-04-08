# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SQLITE_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SQLITE_SRCS ${HDRS_G})

if (POCO_UNBUNDLED)
    find_package(SQLite3)
    set(DATASQLITELIBS ${SQLITE3_LIBRARIES})
    include_directories("${SQLITE3_INCLUDE_DIRS}")
else()
    # sqlite3
    POCO_SOURCES( SQLITE_SRCS sqlite3
        src/sqlite3.c
    )

    POCO_HEADERS( SQLITE_SRCS sqlite3
        src/sqlite3.h
    )

    set(DATASQLITELIBS "")
endif()

if(WINCE)
    add_definitions(-DSQLITE_MSVC_LOCALTIME_API)
endif(WINCE)

add_definitions(-DSQLITE_THREADSAFE=1 -DSQLITE_DISABLE_LFS -DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_COMPLETE -DSQLITE_OMIT_TCL_VARIABLE -DSQLITE_OMIT_DEPRECATED)

add_library(SQLSQLite ${SQLITE_SRCS} )
add_library(Poco::SQLSQLite ALIAS SQLSQLite)
set_target_properties(SQLSQLite
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoSQLSQLite
    DEFINE_SYMBOL SQLite_EXPORTS
    )

target_link_libraries(SQLSQLite Poco::SQL ${DATASQLITELIBS} )
target_include_directories(SQLSQLite
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions(SQLSQLite PUBLIC ${LIB_MODE_DEFINITIONS})

POCO_INSTALL(SQLSQLite)
POCO_GENERATE_PACKAGE(SQLSQLite)

if (POCO_ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()

