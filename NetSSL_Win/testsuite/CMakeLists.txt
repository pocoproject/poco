# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

POCO_SOURCES_AUTO_PLAT(TEST_SRCS OFF
	src/WinDriver.cpp
)

add_executable(NetSSLWin-testrunner ${TEST_SRCS})
set_target_properties(NetSSLWin-testrunner PROPERTIES DEBUG_POSTFIX "d")
# NetSSL_Win is Windows-only, so always use cppignore.win
add_test(
	NAME NetSSLWin
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMAND NetSSLWin-testrunner -ignore ${CMAKE_SOURCE_DIR}/cppignore.win -all
)
target_link_libraries(NetSSLWin-testrunner PUBLIC Poco::NetSSLWin Poco::Util Poco::XML CppUnit)

# Test data
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/any.pfx ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/any.pfx COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/testrunner.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/NetSSLWin-testrunner.xml COPYONLY)
