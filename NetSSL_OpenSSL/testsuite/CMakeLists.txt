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
add_test(NAME ${LIBNAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${TESTUNIT} -all)
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoNetSSL PocoCrypto PocoNet PocoUtil PocoXML PocoFoundation CppUnit)

# The test is run in the build directory. So the test data is copied there too
add_custom_command(TARGET ${TESTUNIT} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/any.pem ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/rootcert.pem ${CMAKE_CURRENT_BINARY_DIR}
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/testrunner.xml ${CMAKE_CURRENT_BINARY_DIR}/${TESTUNIT}.xml
                   )

