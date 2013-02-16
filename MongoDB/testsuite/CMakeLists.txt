set( TEST_SRCS
src/Driver.cpp
src/MongoDBTest.cpp
src/MongoDBTest.h
src/MongoDBTestSuite.cpp
src/MongoDBTestSuite.h
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT}  PocoMongoDB PocoFoundation CppUnit )

