# Sources
file(GLOB SRCS_G "src/*.cpp")
# The server program the ProcessClient tests launch has its own target.
list(FILTER SRCS_G EXCLUDE REGEX "/TestServer\\.cpp$")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

poco_add_executable(AIMCP-testrunner ${TEST_SRCS})
if(ANDROID)
	add_test(
		NAME AIMCP
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/AIMCP-testrunner -DTEST_PARAMETER=-all -P ${PROJECT_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	if(WIN32)
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
	else()
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
	endif()
	add_test(
		NAME AIMCP
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND AIMCP-testrunner -ignore ${CPPIGNORE_FILE} -all
	)
endif()
target_link_libraries(AIMCP-testrunner PUBLIC Poco::AIMCP CppUnit)

# The server program of the ProcessClient tests, built next to the test runner.
poco_add_executable(AIMCPTestServer src/TestServer.cpp)
set_target_properties(AIMCPTestServer PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
target_link_libraries(AIMCPTestServer PUBLIC Poco::AIMCP)
add_dependencies(AIMCP-testrunner AIMCPTestServer)
