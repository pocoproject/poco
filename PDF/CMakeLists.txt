set(LIBNAME "PocoPDF")
<<<<<<< HEAD
set(LIBNAME "${LIBNAME}${LIB_EXT}")
=======
>>>>>>> develop

aux_source_directory(src SRCS)
include_directories(include/Poco/PDF) # zip src

if (NOT POCO_STATIC)
<<<<<<< HEAD
  add_definitions(-DJSON_EXPORTS)
=======
  add_definitions(-DPDF_EXPORTS)
>>>>>>> develop
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
<<<<<<< HEAD
target_link_libraries( ${LIBNAME} PocoFoundation${LIB_EXT})
=======
target_link_libraries( ${LIBNAME} PocoFoundation)
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

