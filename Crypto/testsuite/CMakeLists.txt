set( TEST_SRCS
src/CryptoTest.cpp
src/DigestEngineTest.cpp
src/CryptoTestSuite.cpp
src/Driver.cpp
src/RSATest.cpp
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoCrypto PocoNetSSL PocoXML PocoUtil PocoFoundation CppUnit pthread)

