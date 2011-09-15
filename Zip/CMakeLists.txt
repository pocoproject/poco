set(LIBNAME "PocoZip")
set(LIBNAMED "${LIBNAME}d")

aux_source_directory(src SRCS)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS}
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoUtil PocoXML PocoFoundation)

add_library( ${LIBNAMED} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAMED}  
	PROPERTIES COMPILE_FLAGS "${DEBUG_CXX_FLAGS}" 
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAMED} PocoUtild PocoXMLd PocoFoundationd)

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

