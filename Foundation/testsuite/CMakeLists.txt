set(TESTUNIT "Foundation-testrunner")

# Sources
file(GLOB SRCS_G "src/*.cpp")
file(GLOB SRCS_G_REMOVE
	src/TestApp.cpp
	src/TestLibrary.cpp
	src/TestLibraryMissingDeps.cpp
	src/TestPlugin.cpp
)
# Exclude FastLogger tests if FastLogger is disabled
if(NOT ENABLE_FASTLOGGER)
	list(APPEND SRCS_G_REMOVE
		src/FastLoggerTest.cpp
		src/FastLoggerChannelsTest.cpp
	)
endif()
list(REMOVE_ITEM SRCS_G ${SRCS_G_REMOVE})
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

# WinDriver depends on WinTestRunner which depends on MFC, and we don't want that
POCO_SOURCES_AUTO_PLAT(TEST_SRCS OFF
	src/WinDriver.cpp
)

add_executable(Foundation-testrunner ${TEST_SRCS})
set_target_properties(Foundation-testrunner PROPERTIES DEBUG_POSTFIX "d")
if(ANDROID)
	add_test(
		NAME Foundation
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_COMMAND} -DANDROID_NDK=${ANDROID_NDK} "-DTEST_FILES=${CMAKE_CURRENT_SOURCE_DIR}/data;${CMAKE_BINARY_DIR}/bin/TestApp;${CMAKE_BINARY_DIR}/bin/TestLibrary.so" -DLIBRARY_DIR=${CMAKE_BINARY_DIR}/lib -DUNITTEST=${CMAKE_BINARY_DIR}/bin/Foundation-testrunner -DTEST_PARAMETER=-all -P ${PROJECT_SOURCE_DIR}/cmake/ExecuteOnAndroid.cmake
	)
else()
	if(WIN32)
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
	else()
		set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
	endif()
	add_test(
		NAME Foundation
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND Foundation-testrunner -ignore ${CPPIGNORE_FILE} -all
	)
	set_tests_properties(Foundation PROPERTIES ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")	# The SharedLibaryTest has to look for shared libraries in the working directory
	set_property(TEST Foundation APPEND PROPERTY ENVIRONMENT "PATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}:$ENV{PATH}") # The ProcessTest has to look for the TestApp in the working directory
	set_property(TEST Foundation APPEND PROPERTY ENVIRONMENT "POCO_BASE=${PROJECT_SOURCE_DIR}")
	# The test is run in the runtime directory. So the test data is copied there too
	add_custom_command(
		TARGET Foundation-testrunner POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
	)
endif()

target_link_libraries(Foundation-testrunner PUBLIC Poco::Foundation CppUnit)
if(UNIX AND NOT ANDROID)
	target_link_libraries(Foundation-testrunner PUBLIC pthread)
endif(UNIX AND NOT ANDROID)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	target_link_libraries(Foundation-testrunner PUBLIC atomic)
endif()

# TestApp
add_executable(TestApp src/TestApp.cpp)
# The test is run in the runtime directory. So the TestApp is built there too because it is used by the tests
set_target_properties(TestApp PROPERTIES
	DEBUG_POSTFIX "d"
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
target_link_libraries(TestApp PUBLIC Poco::Foundation)

add_dependencies(Foundation-testrunner TestApp)

# TestLibrary and related shared libraries - only for shared library builds
# SharedLibrary and ClassLoader tests require dynamically loadable libraries
if(BUILD_SHARED_LIBS)
	add_library(TestLibrary SHARED src/TestLibrary.cpp src/TestPlugin.cpp src/TestPlugin.h)
	set_target_properties(TestLibrary PROPERTIES
		DEBUG_POSTFIX "d"
		# The test requires the library named TestLibrary. By default it is prefixed with lib.
		PREFIX ""
	)

	# The test is run in the runtime directory. So the TestLibrary is built there too because it is used by the tests
	set_target_properties(TestLibrary PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	target_link_libraries(TestLibrary PUBLIC Poco::Foundation)

	add_dependencies(Foundation-testrunner TestLibrary)

	# TestLibraryMissingDeps - a library with missing dependencies for testing findMissingDependencies
	# Build a stub library that TestLibraryMissingDeps links against
	# The test will delete it at runtime to simulate a missing dependency
	add_library(NonExistentLibrary SHARED src/NonExistentStub.c)
	set_target_properties(NonExistentLibrary PROPERTIES
		DEBUG_POSTFIX "d"
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
	)
	# For multi-config generators (Visual Studio), set per-configuration archive output
	foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
		set_target_properties(NonExistentLibrary PROPERTIES
			ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER}}
		)
	endforeach()

	add_library(TestLibraryMissingDeps SHARED src/TestLibraryMissingDeps.cpp)
	set_target_properties(TestLibraryMissingDeps PROPERTIES
		DEBUG_POSTFIX "d"
		PREFIX ""
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	)
	target_link_libraries(TestLibraryMissingDeps PUBLIC Poco::Foundation NonExistentLibrary)
	add_dependencies(TestLibraryMissingDeps NonExistentLibrary)

	add_dependencies(Foundation-testrunner TestLibraryMissingDeps)
endif()
