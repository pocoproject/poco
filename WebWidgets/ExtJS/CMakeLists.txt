set(LIBNAME  "PocoExtJS")
set(LIBNAMED "${LIBNAME}d")

include_directories( include src )

set(SRCS "")
aux_source_directory(src SRCS)

add_definitions(-DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_COMPLETE -DSQLITE_OMIT_TCL_VARIABLE)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS}
	VERSION ${SHARED_LIBRARY_VERSION}
	SOVERSION ${SHARED_LIBRARY_VERSION})
target_link_libraries( ${LIBNAME} PocoWebWidgets PocoData )

add_library( ${LIBNAMED} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAMED}  
	PROPERTIES COMPILE_FLAGS "${DEBUG_CXX_FLAGS}" 
	VERSION ${SHARED_LIBRARY_VERSION}
	SOVERSION ${SHARED_LIBRARY_VERSION})
target_link_libraries( ${LIBNAMED} PocoWebWidgetsd PocoDatad )

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )
    
install(
    TARGETS ${LIBNAME} ${LIBNAMED}
    DESTINATION lib
    )

if (ENABLE_TESTS)
  add_subdirectory(samples)
  add_subdirectory(testsuite)
endif ()

