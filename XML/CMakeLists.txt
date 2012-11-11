set(LIBNAME "PocoXML")
set(LIBNAME "${LIBNAME}${LIB_EXT}")

aux_source_directory(src SRCS)

add_definitions(-D_USRDLL -DXML_NS -DXML_DTD -DHAVE_EXPAT_CONFIG_H -DXML_EXPORTS)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES 
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoFoundation${LIB_EXT})

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

