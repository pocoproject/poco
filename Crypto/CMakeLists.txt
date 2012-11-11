set(LIBNAME "PocoCrypto")
set(LIBNAME "${LIBNAME}${LIB_EXT}")

aux_source_directory(src SRCS)
add_definitions(-D_USRDLL -DCrypto_EXPORTS)
include_directories( include) 

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} PROPERTIES VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )

#if(CMAKE_SYSTEM MATCHES "Windows")
#    target_link_libraries( ${LIBNAME} PocoFoundation${LIB_EXT} ${LIB_EAY_RELEASE} ${OPENSSL_LIBRARIES}  )
#ELSE()
#    target_link_libraries( ${LIBNAME} PocoFoundation${LIB_EXT} ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} )
#ENDIF()

target_link_libraries( ${LIBNAME} PocoFoundation${LIB_EXT} ${OPENSSL_LIBRARIES}  )

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
