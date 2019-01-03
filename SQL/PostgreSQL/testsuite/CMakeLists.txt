# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h" )
POCO_HEADERS_AUTO( TEST_SRCS ${HDRS_G})

POCO_SOURCES_AUTO_PLAT( TEST_SRCS OFF
    src/WinDriver.cpp
)

add_executable(SQLPostgreSQL-testrunner ${TEST_SRCS} )
add_test(NAME SQLPostgreSQL WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND SQLPostgreSQL-testrunner -all)
target_link_libraries(SQLPostgreSQL-testrunner PUBLIC Poco::SQLPostgreSQL Poco::CppUnit)
