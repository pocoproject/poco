set( TEST_SRCS
src/Driver.cpp
src/HTTPSClientSessionTest.cpp
src/HTTPSClientTestSuite.cpp
src/HTTPSServerTest.cpp
src/HTTPSServerTestSuite.cpp
src/HTTPSStreamFactoryTest.cpp
src/HTTPSTestServer.cpp
src/NetSSLTestSuite.cpp
src/TCPServerTest.cpp
src/TCPServerTestSuite.cpp
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoNetSSL PocoCrypto PocoNet PocoUtil PocoXML PocoFoundation CppUnit)

