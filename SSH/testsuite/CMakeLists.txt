file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

add_executable(SSH-testrunner ${TEST_SRCS})
set_target_properties(SSH-testrunner PROPERTIES DEBUG_POSTFIX "d")

target_link_libraries(SSH-testrunner PUBLIC Poco::SSH Poco::Foundation CppUnit)
target_include_directories(SSH-testrunner PRIVATE ${LIBSSH_INCLUDE_DIRS})

add_test(
	NAME SSH
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND SSH-testrunner -all
)
set_tests_properties(SSH PROPERTIES ENVIRONMENT POCO_BASE=${PROJECT_SOURCE_DIR})
