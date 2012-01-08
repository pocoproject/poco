set(LIBNAME  "PocoExtJS")
set(LIBNAME "${LIBNAME}${LIB_EXT}")

include_directories( include src )

set(SRCS "")
aux_source_directory(src SRCS)

if (NOT POCO_STATIC)
  add_definitions(-DExtJS_EXPORTS -DPOCO_DLL)
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION}
	SOVERSION ${SHARED_LIBRARY_VERSION})
target_link_libraries( ${LIBNAME} PocoWebWidgets${LIB_EXT} )

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

