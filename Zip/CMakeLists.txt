set(LIBNAME "PocoZip")
<<<<<<< HEAD
set(LIBNAME "${LIBNAME}${LIB_EXT}")
=======
>>>>>>> develop

aux_source_directory(src SRCS)

if (NOT POCO_STATIC)
  add_definitions(-DZip_EXPORTS)
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
<<<<<<< HEAD
target_link_libraries( ${LIBNAME} PocoUtil${LIB_EXT} PocoXML${LIB_EXT} PocoFoundation${LIB_EXT})
=======
target_link_libraries( ${LIBNAME} PocoUtil PocoXML PocoFoundation)
>>>>>>> develop

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
  add_subdirectory(samples)
  add_subdirectory(testsuite)
endif ()

