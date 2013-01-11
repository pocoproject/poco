set(LIBNAME "PocoNetSSL")
<<<<<<< HEAD
set(LIBNAME "${LIBNAME}${LIB_EXT}")
=======
>>>>>>> develop

aux_source_directory(src SRCS)
include_directories( ${OPENSSL_INCLUDE_DIR} )

if (NOT POCO_STATIC)
  add_definitions(-DNetSSL_EXPORTS)
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
<<<<<<< HEAD
target_link_libraries( ${LIBNAME} PocoCrypto${LIB_EXT} PocoNet${LIB_EXT} PocoUtil${LIB_EXT} PocoFoundation${LIB_EXT} ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} )
=======
target_link_libraries( ${LIBNAME} PocoCrypto PocoNet PocoUtil PocoFoundation ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} )
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

