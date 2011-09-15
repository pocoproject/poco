set(LIBNAME "CppUnit")
set(LIBNAMED "${LIBNAME}d")

aux_source_directory(src SRCS)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS}
	VERSION "1"
	SOVERSION "1")
target_link_libraries( ${LIBNAME} )

add_library( ${LIBNAMED} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAMED}  
	PROPERTIES COMPILE_FLAGS "${DEBUG_CXX_FLAGS}" 
	VERSION "1"
	SOVERSION "1")
target_link_libraries( ${LIBNAMED} )
