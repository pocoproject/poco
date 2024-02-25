# Sources
file(GLOB SRCS_G "src/*.cpp")
LIST(REMOVE_ITEM SRCS_G "${CMAKE_CURRENT_SOURCE_DIR}/src/SQLExecutor.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

POCO_SOURCES_AUTO_PLAT(TEST_SRCS OFF
	src/WinDriver.cpp
)

add_executable(Data-testrunner ${TEST_SRCS})
if(ANDROID)
	add_test(
		NAME Data
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/Data-testrunner -DTEST_PARAMETER=-all -P ${CMAKE_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	add_test(
		NAME Data
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND Data-testrunner -ignore ${CMAKE_SOURCE_DIR}/cppignore.lnx -all
	)
	set_tests_properties(Data PROPERTIES ENVIRONMENT POCO_BASE=${CMAKE_SOURCE_DIR})
endif()
target_link_libraries(Data-testrunner PUBLIC Poco::DataTest Poco::Data CppUnit)

if(NOT POCO_DATA_NO_SQL_PARSER)
	target_include_directories(Data-testrunner
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SQLParser/src
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SQLParser
	)
endif()
