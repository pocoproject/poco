set(LIBNAME "mod_poco")
set(LIBNAMED "${LIBNAME}d")

include_directories( include ${APACHE_INCLUDE_DIR} ${APRUTIL_INCLUDE_DIR} )

aux_source_directory(src SRCS)

add_library( ${LIBNAME} SHARED ${SRCS} )
set_target_properties( ${LIBNAME} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} )

add_library( ${LIBNAMED} SHARED ${SRCS} )
set_target_properties( ${LIBNAMED} PROPERTIES COMPILE_FLAGS ${DEBUG_CXX_FLAGS} VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAMED} )

if (ENABLE_TESTS)
  add_subdirectory(samples)
endif ()