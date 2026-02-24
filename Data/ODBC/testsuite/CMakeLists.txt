# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

POCO_SOURCES_AUTO_PLAT(TEST_SRCS OFF
	src/WinDriver.cpp
)

add_executable(DataODBC-testrunner ${TEST_SRCS})
set_target_properties(DataODBC-testrunner PROPERTIES DEBUG_POSTFIX "d")
if(ANDROID)
	add_test(
		NAME DataODBC
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/DataODBC-testrunner -DTEST_PARAMETER=-all -P ${PROJECT_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	if(WIN32)
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
	else()
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
	endif()
	add_test(
		NAME DataODBC
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND DataODBC-testrunner -ignore ${CPPIGNORE_FILE} -all
	)
	set_tests_properties(DataODBC PROPERTIES ENVIRONMENT POCO_BASE=${PROJECT_SOURCE_DIR})
endif()

target_link_libraries(DataODBC-testrunner PUBLIC Poco::DataTest Poco::DataODBC Poco::Data CppUnit)
target_include_directories(DataODBC-testrunner PUBLIC ${PROJECT_SOURCE_DIR}/Data/DataTest/include/)
