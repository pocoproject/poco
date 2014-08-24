set(LIBNAME  "PocoDataSQLite")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SQLITE_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SQLITE_SRCS ${HDRS_G})

include_directories( "include" "src" )

if (POCO_UNBUNDLED)
    find_package(SQLite3)
    set(DATASQLITELIBS PocoData PocoFoundation ${SQLITE3_LIBRARIES})
    include_directories("${SQLITE3_INCLUDE_DIRS}")
else()
    # sqlite3
    POCO_SOURCES( SQLITE_SRCS sqlite3
        src/sqlite3.c
    )

    POCO_HEADERS( SQLITE_SRCS sqlite3
        src/sqlite3.h
    )

    set(DATASQLITELIBS PocoData PocoFoundation)
endif()

add_definitions(-DSQLITE_THREADSAFE=1 -DSQLITE_DISABLE_LFS -DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_COMPLETE -DSQLITE_OMIT_TCL_VARIABLE -DSQLITE_OMIT_DEPRECATED)

add_library( ${LIBNAME} ${LIB_MODE} ${SQLITE_SRCS} )
set_target_properties( ${LIBNAME}
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    DEFINE_SYMBOL SQLite_EXPORTS)
target_link_libraries( ${LIBNAME} ${DATASQLITELIBS} )

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )

install(
    TARGETS ${LIBNAME}
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin
    )

if (ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()

