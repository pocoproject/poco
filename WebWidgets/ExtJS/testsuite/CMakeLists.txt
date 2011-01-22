set( TEST_SRCS
src/Driver.cpp
src/ExtJSTest.cpp
src/ExtJSTest.h
src/ExtJSTestSuite.cpp
src/ExtJSTestSuite.h
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoExtJS PocoWebWidgets PocoNet PocoUtil PocoXML PocoFoundation CppUnit)

