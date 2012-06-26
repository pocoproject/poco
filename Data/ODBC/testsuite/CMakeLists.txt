set( TEST_SRCS
src/Driver.cpp
src/ODBCAccessTest.cpp
src/ODBCDB2Test.cpp
src/ODBCMySQLTest.cpp
src/ODBCOracleTest.cpp
src/ODBCPostgreSQLTest.cpp
src/ODBCSQLServerTest.cpp
src/ODBCSQLiteTest.cpp
src/ODBCTest.cpp
src/ODBCTestSuite.cpp
src/SQLExecutor.cpp
)

#include_directories()

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoDataODBC PocoData PocoFoundation CppUnit )

