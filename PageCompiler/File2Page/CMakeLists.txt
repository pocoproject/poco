# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

add_executable(File2Page ${SRCS})
set_target_properties(File2Page
	PROPERTIES
	OUTPUT_NAME f2cpsp
)

target_link_libraries(File2Page PUBLIC Poco::Util)

install(
	TARGETS File2Page EXPORT File2PageTargets
	LIBRARY DESTINATION lib${LIB_SUFFIX}
	ARCHIVE DESTINATION lib${LIB_SUFFIX}
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
