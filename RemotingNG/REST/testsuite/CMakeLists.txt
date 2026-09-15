# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

poco_add_executable(RemotingNGREST-testrunner ${TEST_SRCS})
if(ANDROID)
	add_test(
		NAME RemotingNGREST
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/RemotingNGREST-testrunner -DTEST_PARAMETER=-all -P ${PROJECT_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	if(WIN32)
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
	else()
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
	endif()
	add_test(
		NAME RemotingNGREST
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND RemotingNGREST-testrunner -ignore ${CPPIGNORE_FILE} -all
	)
	set_tests_properties(RemotingNGREST PROPERTIES ENVIRONMENT POCO_BASE=${PROJECT_SOURCE_DIR})
endif()

target_link_libraries(RemotingNGREST-testrunner PUBLIC Poco::RemotingNGREST Poco::RemotingNG Poco::Net CppUnit)
target_include_directories(RemotingNGREST-testrunner
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
