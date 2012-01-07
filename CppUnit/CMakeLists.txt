set(LIBNAME "CppUnit")

aux_source_directory(src SRCS)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES
	VERSION "1"
	SOVERSION "1")
target_link_libraries( ${LIBNAME} )

