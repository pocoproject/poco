set(LIBNAME "PocoNetSSL")

set(SRCS
    src/AcceptCertificateHandler.cpp
    src/CertificateHandlerFactory.cpp
    src/CertificateHandlerFactoryMgr.cpp
    src/ConsoleCertificateHandler.cpp
    src/Context.cpp
    src/HTTPSClientSession.cpp
    src/HTTPSSessionInstantiator.cpp
    src/HTTPSStreamFactory.cpp
    src/InvalidCertificateHandler.cpp
    src/KeyConsoleHandler.cpp
    src/KeyFileHandler.cpp
    src/PrivateKeyFactory.cpp
    src/PrivateKeyFactoryMgr.cpp
    src/PrivateKeyPassphraseHandler.cpp
    src/RejectCertificateHandler.cpp
    src/SecureServerSocket.cpp
    src/SecureServerSocketImpl.cpp
    src/SecureSMTPClientSession.cpp
    src/SecureSocketImpl.cpp
    src/SecureStreamSocket.cpp
    src/SecureStreamSocketImpl.cpp
    src/Session.cpp
    src/SSLException.cpp
    src/SSLManager.cpp
    src/Utility.cpp
    src/VerificationErrorArgs.cpp
    src/X509Certificate.cpp
)

include_directories( ${OPENSSL_INCLUDE_DIR} )

if (NOT POCO_STATIC)
  add_definitions(-DNetSSL_EXPORTS)
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME}
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoCrypto PocoNet PocoUtil PocoFoundation ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY} )

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

