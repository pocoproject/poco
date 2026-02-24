# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
file(GLOB HDRS_E ${PROJECT_SOURCE_DIR}/Data/DataTest/include/*.h)
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_E})

POCO_SOURCES_AUTO_PLAT(TEST_SRCS OFF
	src/WinDriver.cpp
)

add_executable(DataSQLite-testrunner ${TEST_SRCS})
set_target_properties(DataSQLite-testrunner PROPERTIES DEBUG_POSTFIX "d")
if(ANDROID)
	add_test(
		NAME DataSQLite
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/DataSQLite-testrunner -DTEST_PARAMETER=-all -P ${PROJECT_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	if(WIN32)
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
	else()
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
	endif()
	add_test(
		NAME DataSQLite
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND DataSQLite-testrunner -ignore ${CPPIGNORE_FILE} -all
	)
	set_tests_properties(DataSQLite PROPERTIES ENVIRONMENT POCO_BASE=${PROJECT_SOURCE_DIR})
endif()

target_link_libraries(DataSQLite-testrunner PUBLIC Poco::DataSQLite Poco::DataTest CppUnit)
target_include_directories(DataSQLite-testrunner PUBLIC ${PROJECT_SOURCE_DIR}/Data/DataTest/include/)
