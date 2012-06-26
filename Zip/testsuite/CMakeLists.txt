set( TEST_SRCS
src/CompressTest.cpp
src/Driver.cpp
src/PartialStreamTest.cpp
src/ZipTest.cpp
src/ZipTestSuite.cpp
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT}  PocoZip PocoNet PocoFoundation CppUnit )

