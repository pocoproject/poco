set(LIBNAME "PocoDataMySQL")

set(SRCS "")
aux_source_directory(src SRCS)

add_definitions(-DTHREADSAFE -DNO_TCL)

include_directories( include ${MYSQL_INCLUDE_DIR})
#link_directories( )

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoData PocoFoundation ${MYSQL_LIB})

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
