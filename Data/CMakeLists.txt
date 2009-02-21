set(LIBNAME "PocoData")
set(LIBNAMED "${LIBNAME}d")

set(SRCS "")
aux_source_directory(src SRCS)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES COMPILE_FLAGS ${DEBUG_CXX_FLAGS}
	VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}
	SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
target_link_libraries( ${LIBNAME} PocoFoundation )

add_library( ${LIBNAMED} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAMED}  
	PROPERTIES COMPILE_FLAGS "${RELEASE_CXX_FLAGS}" 
	VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}
	SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
target_link_libraries( ${LIBNAMED} PocoFoundationd )

install(
    DIRECTORY include/Poco
    DESTINATION include
    )
    
install(
    TARGETS ${LIBNAME} ${LIBNAMED}
    DESTINATION lib
    )

add_subdirectory( SQLite )
if(MYSQL_FOUND)
    add_subdirectory( MySQL )
endif(MYSQL_FOUND)
if(ODBC_CONFIG)
#    add_subdirectory( ODBC )
endif(ODBC_CONFIG)

add_subdirectory( samples )
add_subdirectory( testsuite )
