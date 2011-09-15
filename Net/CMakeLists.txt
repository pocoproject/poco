set(LIBNAME "PocoNet")
set(LIBNAMED "${LIBNAME}d")

aux_source_directory(src BASE_SRCS)

set( WIN_SRCS
)

set( LIN_SRCS
)

if(CMAKE_SYSTEM MATCHES "Windows")
 set(SRCS ${BASE_SRCS} ${WIN_SRCS})
 set(SYSLIBS ${SYSLIBS} ws2_32) 
else (CMAKE_SYSTEM MATCHES "Windows")
 set(SRCS ${BASE_SRCS} ${LIN_SRCS})
endif(CMAKE_SYSTEM MATCHES "Windows")

if (NOT POCO_STATIC)
  add_definitions(-DNet_EXPORTS)
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS}
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoFoundation ${SYSLIBS})

add_library( ${LIBNAMED} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAMED}  
	PROPERTIES COMPILE_FLAGS "${DEBUG_CXX_FLAGS}" 
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAMED} PocoFoundationd ${SYSLIBS})

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

