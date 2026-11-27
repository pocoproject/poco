# Sources
file(GLOB SRCS_G "src/*.cpp")
# The program that UtilityTest starts in place of a preprocessor has its own target.
list(FILTER SRCS_G EXCLUDE REGEX "/TestPreprocessor\\.cpp$")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

poco_add_executable(CppParser-testrunner ${TEST_SRCS})
if(ANDROID)
	add_test(
		NAME CppParser
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} "-DTEST_FILES=${CMAKE_BINARY_DIR}/bin/CppParserTestPreprocessor" -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/CppParser-testrunner -DTEST_PARAMETER=-all -P ${PROJECT_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	if(WIN32)
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
	else()
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
	endif()
	add_test(
		NAME CppParser
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND CppParser-testrunner -ignore ${CPPIGNORE_FILE} -all
	)
endif()
target_link_libraries(CppParser-testrunner PUBLIC Poco::CppParser CppUnit)

# The program that UtilityTest starts in place of a preprocessor, built next to the test runner.
poco_add_executable(CppParserTestPreprocessor src/TestPreprocessor.cpp)
set_target_properties(CppParserTestPreprocessor PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
add_dependencies(CppParser-testrunner CppParserTestPreprocessor)
