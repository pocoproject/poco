set(LIBNAME "mod_poco")

include_directories( include ${APACHE_INCLUDE_DIR} ${APRUTIL_INCLUDE_DIR} )

aux_source_directory(src SRCS)

add_library( ${LIBNAME} SHARED ${SRCS} )
set_target_properties( ${LIBNAME} PROPERTIES VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} )

if (ENABLE_TESTS)
  add_subdirectory(samples)
endif ()