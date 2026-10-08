# PageCompiler must run on the build host to generate the test sources.
if(CMAKE_CROSSCOMPILING)
	return()
endif()

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(TEST_SRCS ${HDRS_G})

if(WIN32)
	set(PAGECOMPILER_OUTPUT_OPTION "/output-dir=${CMAKE_CURRENT_BINARY_DIR}")
	set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.win")
else()
	set(PAGECOMPILER_OUTPUT_OPTION "--output-dir=${CMAKE_CURRENT_BINARY_DIR}")
	set(CPPIGNORE_FILE "${PROJECT_SOURCE_DIR}/cppignore.lnx")
endif()

# Output file names are determined by each page's class attribute.
function(pagecompiler_test_page page handler)
	set(page "${CMAKE_CURRENT_SOURCE_DIR}/${page}")
	set(source "${CMAKE_CURRENT_BINARY_DIR}/${handler}.cpp")
	set(header "${CMAKE_CURRENT_BINARY_DIR}/${handler}.h")
	add_custom_command(
		OUTPUT "${source}" "${header}"
		COMMAND PageCompiler "${PAGECOMPILER_OUTPUT_OPTION}" "${page}"
		DEPENDS PageCompiler "${page}"
		VERBATIM
	)
	set(TEST_SRCS ${TEST_SRCS} "${source}" "${header}" PARENT_SCOPE)
endfunction()

pagecompiler_test_page(../test/test.cpsp TestHandler)
pagecompiler_test_page(data/test-no-form.cpsp TestHandlerNoForm)
pagecompiler_test_page(data/test-stringify.cpsp StringifyHandler)
pagecompiler_test_page(data/test-stringify-no-form.cpsp StringifyHandlerNoForm)
pagecompiler_test_page(data/test-stringify-escape.cpsp StringifyEscapeHandler)

poco_add_executable(PageCompiler-testrunner ${TEST_SRCS})
target_include_directories(PageCompiler-testrunner PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(PageCompiler-testrunner PUBLIC Poco::Net CppUnit)

# Why: exercite the PageCompiler functionality through the test runner, like [[maybe_unused]] in C++ code.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(PageCompiler-testrunner PRIVATE
		$<$<COMPILE_LANGUAGE:CXX>:-Wunused-parameter>
	)
endif()

add_test(
	NAME PageCompiler
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND PageCompiler-testrunner -ignore ${CPPIGNORE_FILE} -all
)
set_tests_properties(PageCompiler PROPERTIES ENVIRONMENT POCO_BASE=${PROJECT_SOURCE_DIR})
