# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

add_executable(EncodingsCompiler ${SRCS})
set_target_properties(EncodingsCompiler
	PROPERTIES
	OUTPUT_NAME tec
)

target_link_libraries(EncodingsCompiler PUBLIC Poco::Net Poco::Util)

install(
	TARGETS EncodingsCompiler EXPORT "EncodingsCompilerTargets"
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
