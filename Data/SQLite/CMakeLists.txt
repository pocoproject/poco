set(LIBNAME  "PocoDataSQLite")
set(LIBNAME "${LIBNAME}${LIB_EXT}")

include_directories( include src )

set(SRCS 
	src/Binder.cpp
	src/Connector.cpp
	src/Extractor.cpp
	src/SQLiteException.cpp
	src/SQLiteStatementImpl.cpp
	src/SessionImpl.cpp
	src/Utility.cpp
)


if (!POCO_UNBUNDLED)
	set(SRCS "${SRCS} src/sqlite3.c")
	set(DATASQLITELIBS PocoData${LIB_EXT} PocoFoundation${LIB_EXT} )
else()
	set(DATASQLITELIBS PocoData${LIB_EXT} PocoFoundation${LIB_EXT} sqlite3)
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
    DESTINATION lib
    )

if (ENABLE_TESTS)
  add_subdirectory(testsuite)
endif ()

