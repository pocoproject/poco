set(LIBNAME  "PocoDataSQLite")

set(SRCS
    src/Binder.cpp
    src/Connector.cpp
    src/Extractor.cpp
    src/Notifier.cpp
    src/SessionImpl.cpp
    src/SQLiteException.cpp
    src/SQLiteStatementImpl.cpp
    src/Utility.cpp
)

include_directories( "include" src )

if (POCO_UNBUNDLED)
    find_package(SQLite3)
    set(DATASQLITELIBS PocoData PocoFoundation ${SQLITE3_LIBRARIES})
    include_directories(${SQLITE3_INCLUDE_DIRS})
else()
    set(SRCS ${SRCS} src/sqlite3.c)
    set(DATASQLITELIBS PocoData PocoFoundation)
endif()

add_definitions(-DSQLITE_THREADSAFE=1 -DSQLITE_DISABLE_LFS -DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_COMPLETE -DSQLITE_OMIT_TCL_VARIABLE -DSQLITE_OMIT_DEPRECATED)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME}
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
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

