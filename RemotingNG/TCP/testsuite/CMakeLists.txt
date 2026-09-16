# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

poco_add_executable(RemotingNGTCP-testrunner ${TEST_SRCS})
if(ANDROID)
	add_test(
		NAME RemotingNGTCP
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/RemotingNGTCP-testrunner -DTEST_PARAMETER=-all -P ${PROJECT_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	if(WIN32)
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
	else()
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
	endif()
	add_test(
		NAME RemotingNGTCP
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND RemotingNGTCP-testrunner -ignore ${CPPIGNORE_FILE} -all
	)
	set_tests_properties(RemotingNGTCP PROPERTIES ENVIRONMENT POCO_BASE=${PROJECT_SOURCE_DIR})
endif()

target_link_libraries(RemotingNGTCP-testrunner PUBLIC Poco::RemotingNGTCP Poco::RemotingNG Poco::Net CppUnit)
target_include_directories(RemotingNGTCP-testrunner
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
