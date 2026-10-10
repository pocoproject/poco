# Test data
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

poco_add_executable(Crypto-testrunner ${TEST_SRCS})
if(ANDROID)
	add_test(
		NAME Crypto
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/Crypto-testrunner -DTEST_PARAMETER=-all -P ${PROJECT_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	if(WIN32)
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
	else()
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
	endif()
	add_test(
		NAME Crypto
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND Crypto-testrunner -ignore ${CPPIGNORE_FILE} -all
	)
	if(OPENSSL_VERSION VERSION_GREATER_EQUAL 3.0)
		# testLegacyProviderErrorQueue can only fail when the legacy provider cannot be loaded.
		file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/no-openssl-modules)
		add_test(
			NAME Crypto-legacy-provider-unavailable
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMAND Crypto-testrunner CryptoTest::testLegacyProviderErrorQueue
		)
		set_tests_properties(Crypto-legacy-provider-unavailable PROPERTIES
			ENVIRONMENT "OPENSSL_MODULES=${CMAKE_CURRENT_BINARY_DIR}/no-openssl-modules")
	endif()
endif()
target_link_libraries(Crypto-testrunner PUBLIC Poco::Crypto CppUnit)
if(MSVC)
	target_link_libraries(Crypto-testrunner PRIVATE OpenSSL::applink)
endif()
if(UNIX AND NOT ANDROID)
	target_link_libraries(Crypto-testrunner PUBLIC pthread)
endif(UNIX AND NOT ANDROID)
