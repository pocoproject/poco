# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

POCO_SOURCES_AUTO_PLAT(TEST_SRCS OFF
	src/WinDriver.cpp
)

add_executable(NetSSL-testrunner ${TEST_SRCS})

if(ANDROID)
	add_test(
		NAME NetSSL
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} "-DTEST_FILES=${CMAKE_CURRENT_SOURCE_DIR}/any.pem;${CMAKE_CURRENT_SOURCE_DIR}/rootcert.pem;${CMAKE_CURRENT_SOURCE_DIR}/testrunner.xml" -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/NetSSL-testrunner -DTEST_PARAMETER=-all -P ${CMAKE_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	add_test(
		NAME NetSSL
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND NetSSL-testrunner -ignore ${CMAKE_SOURCE_DIR}/cppignore.lnx -all
	)
	set_tests_properties(NetSSL PROPERTIES ENVIRONMENT POCO_BASE=${CMAKE_SOURCE_DIR})
	# The test is run in the build directory. So the test data is copied there too
	add_custom_command(
		TARGET NetSSL-testrunner POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/any.pem ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/rootcert.pem ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/dhparams.pem ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/testrunner.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/NetSSL-testrunner.xml
	)
endif()
target_link_libraries(NetSSL-testrunner PUBLIC Poco::NetSSL Poco::Util Poco::XML CppUnit)


if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ping/websocket-server.cpp)
	add_executable(NetSSL-server ping/websocket-server.cpp)
	target_link_libraries(NetSSL-server PUBLIC Poco::NetSSL Poco::Util)
	if (NOT ANDROID)
		add_custom_command(
			TARGET NetSSL-server POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/testrunner.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/NetSSL-server.xml
		)
	endif()
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ping/websocket-client.cpp)
	add_executable(NetSSL-client ping/websocket-client.cpp)
	target_link_libraries(NetSSL-client PUBLIC Poco::NetSSL Poco::Util)
	if (NOT ANDROID)
	add_custom_command(
		TARGET NetSSL-client POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/testrunner.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/NetSSL-client.xml
	)
	endif()
endif()
