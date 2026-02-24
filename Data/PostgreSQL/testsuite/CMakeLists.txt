# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

POCO_SOURCES_AUTO_PLAT(TEST_SRCS OFF
	src/WinDriver.cpp
)

add_executable(DataPostgreSQL-testrunner ${TEST_SRCS})
set_target_properties(DataPostgreSQL-testrunner PROPERTIES DEBUG_POSTFIX "d")
if(WIN32)
	set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
else()
	set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
endif()
add_test(
	NAME DataPostgreSQL
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND DataPostgreSQL-testrunner -ignore ${CPPIGNORE_FILE} -all
)
set_tests_properties(DataPostgreSQL PROPERTIES ENVIRONMENT POCO_BASE=${PROJECT_SOURCE_DIR})
target_link_libraries(DataPostgreSQL-testrunner PUBLIC Poco::DataPostgreSQL Poco::DataTest CppUnit)
target_include_directories(DataPostgreSQL-testrunner PUBLIC ${PROJECT_SOURCE_DIR}/Data/DataTest/include/)
