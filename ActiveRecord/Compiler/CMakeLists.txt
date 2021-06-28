# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Version Resource
if(MSVC)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_executable(ActiveRecordCompiler ${SRCS})
set_target_properties(ActiveRecordCompiler
	PROPERTIES
	OUTPUT_NAME arc
)

target_link_libraries(ActiveRecordCompiler PUBLIC Poco::Foundation Poco::Util)

install(
	TARGETS ActiveRecordCompiler EXPORT "ActiveRecordCompiler"
	LIBRARY DESTINATION lib${LIB_SUFFIX}
	ARCHIVE DESTINATION lib${LIB_SUFFIX}
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
