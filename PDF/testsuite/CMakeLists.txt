set( TEST_SRCS
src/Driver.cpp
src/PDFTest.cpp
src/PDFTest.h
src/PDFTestSuite.cpp
src/PDFTestSuite.h
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT}  PocoPDF PocoFoundation CppUnit )

