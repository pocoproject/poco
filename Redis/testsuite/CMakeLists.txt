# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

POCO_SOURCES_AUTO_PLAT(TEST_SRCS OFF
	src/WinDriver.cpp
)

add_executable(Redis-testrunner ${TEST_SRCS})
if(ANDROID)
	add_test(
		NAME Redis
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/Redis-testrunner -DTEST_PARAMETER=-all -P ${CMAKE_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	add_test(
		NAME Redis
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND Redis-testrunner -ignore ${CMAKE_SOURCE_DIR}/cppignore.lnx -all
	)
	set_tests_properties(Redis PROPERTIES ENVIRONMENT POCO_BASE=${CMAKE_SOURCE_DIR})
endif()

target_link_libraries(Redis-testrunner PUBLIC ${CMAKE_THREAD_LIBS_INIT}	 Poco::Redis CppUnit)

if(OLD_REDIS_VERSION)
	target_compile_definitions(Redis-testrunner PRIVATE OLD_REDIS_VERSION)
endif(OLD_REDIS_VERSION)
