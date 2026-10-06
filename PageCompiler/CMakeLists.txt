set(POCO_EXENAME "PageCompiler")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Version Resource
if(MSVC)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

poco_add_executable(PageCompiler ${SRCS})
set_target_properties(PageCompiler
	PROPERTIES
	OUTPUT_NAME cpspc
)

target_link_libraries(PageCompiler PUBLIC Poco::Net Poco::Util)

install(
	TARGETS PageCompiler EXPORT PageCompilerTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


if(ENABLE_TESTS)
	add_test(
		NAME PageCompiler-TestBase
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND PageCompiler -o "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpsp"
	)
	add_test(
		NAME PageCompiler-Stringify
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND PageCompiler -o "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/test/test-stringify.cpsp"
	)
	add_test(
		NAME PageCompiler-StringifyNoForm
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND PageCompiler -o "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/test/test-stringify-no-form.cpsp"
	)
endif()

