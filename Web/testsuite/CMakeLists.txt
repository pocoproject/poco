set( TEST_SRCS
src/Driver.cpp
src/JSONTest.cpp
src/JSONTest.h
src/SOAPTest.cpp
src/SOAPTest.h
src/WebTestSuite.cpp
src/WebTestSuite.h
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoWeb PocoNet PocoUtil PocoXML PocoFoundation CppUnit)

