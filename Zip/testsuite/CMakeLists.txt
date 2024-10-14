# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

POCO_SOURCES_AUTO_PLAT(TEST_SRCS OFF
	src/WinDriver.cpp
)

add_executable(Zip-testrunner ${TEST_SRCS})
if(ANDROID)
	add_test(
		NAME Zip
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} "-DTEST_FILES=${CMAKE_CURRENT_SOURCE_DIR}/data;" -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/Zip-testrunner -DTEST_PARAMETER=-all -P ${CMAKE_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	add_test(
		NAME Zip
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND Zip-testrunner -ignore ${CMAKE_SOURCE_DIR}/cppignore.lnx -all
	)
	set_tests_properties(Zip PROPERTIES ENVIRONMENT POCO_BASE=${CMAKE_SOURCE_DIR})
	# The test is run in the build directory. So the test data is copied there too
	add_custom_command(
		TARGET Zip-testrunner POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
	)
endif()
target_link_libraries(Zip-testrunner PUBLIC Poco::Zip Poco::Net CppUnit)
