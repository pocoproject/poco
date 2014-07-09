set(LIBNAME "PocoCrypto")

set(SRCS
    src/Cipher.cpp
    src/CipherFactory.cpp
    src/CipherImpl.cpp
    src/CipherKey.cpp
    src/CipherKeyImpl.cpp
    src/CryptoStream.cpp
    src/CryptoTransform.cpp
    src/DigestEngine.cpp
    src/OpenSSLInitializer.cpp
    src/RSACipherImpl.cpp
    src/RSADigestEngine.cpp
    src/RSAKey.cpp
    src/RSAKeyImpl.cpp
    src/X509Certificate.cpp
)
add_definitions(-D_USRDLL -DCrypto_EXPORTS)
include_directories( include ${OPENSSL_INCLUDE_DIR} )

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} PROPERTIES VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )

target_link_libraries( ${LIBNAME} PocoFoundation ${OPENSSL_LIBRARIES} )

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )

install(
    TARGETS ${LIBNAME}
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin
    )

if (ENABLE_TESTS)
  add_subdirectory(samples)
  add_subdirectory(testsuite)
endif ()
