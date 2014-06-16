set( TEST_SRCS
src/Binder.cpp
src/Connector.cpp
src/DataTest.cpp
src/Driver.cpp
src/DataTestSuite.cpp
src/Extractor.cpp
src/Preparator.cpp
src/SessionImpl.cpp
src/SessionPoolTest.cpp
src/TestStatementImpl.cpp
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
add_test(NAME ${LIBNAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${TESTUNIT} -all)
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoData PocoUtil PocoXML PocoFoundation CppUnit)

