set(LIBNAME  "PocoDataSQLite")
set(LIBNAME "${LIBNAME}${LIB_EXT}")

include_directories( include src )

set(SRCS "")
aux_source_directory(src SRCS)

add_definitions(-DSQLITE_THREADSAFE=1 -DSQLITE_DISABLE_LFS -DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_COMPLETE -DSQLITE_OMIT_TCL_VARIABLE -DSQLITE_OMIT_DEPRECATED)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoData${LIB_EXT} PocoFoundation${LIB_EXT})

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

