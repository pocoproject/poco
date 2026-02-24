# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

POCO_SOURCES_AUTO_PLAT(TEST_SRCS OFF
	src/WinDriver.cpp
)

add_executable(JWT-testrunner ${TEST_SRCS})
set_target_properties(JWT-testrunner PROPERTIES DEBUG_POSTFIX "d")
if(ANDROID)
	add_test(
		NAME JWT
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/JWT-testrunner -DTEST_PARAMETER=-all -P ${PROJECT_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	if(WIN32)
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
	else()
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
	endif()
	add_test(
		NAME JWT
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND JWT-testrunner -ignore ${CPPIGNORE_FILE} -all
	)
	set_tests_properties(JWT PROPERTIES ENVIRONMENT POCO_BASE=${PROJECT_SOURCE_DIR})
endif()
target_link_libraries(JWT-testrunner PUBLIC Poco::JWT Poco::Crypto CppUnit)
if(MSVC)
	target_link_libraries(JWT-testrunner PRIVATE OpenSSL::applink)
endif()
if(UNIX AND NOT ANDROID)
	target_link_libraries(JWT-testrunner PUBLIC pthread)
endif(UNIX AND NOT ANDROID)
