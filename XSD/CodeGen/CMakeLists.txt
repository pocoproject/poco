# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Version Resource
if(MSVC)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/ProDLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/ProDLLVersion.rc)
endif()

add_executable(CodeGen ${SRCS})
set_target_properties(CodeGen
	PROPERTIES
	OUTPUT_NAME XSDGen
)

if(ENABLE_NETSSL)
	target_compile_definitions(CodeGen PUBLIC POCO_XSD_ENABLE_HTTPS)
	set(ADD_TARGET_LIBS Poco::NetSSL)
else()
	if(ENABLE_NETSSL_WIN)
		target_compile_definitions(CodeGen PUBLIC POCO_XSD_ENABLE_HTTPS)
		set(ADD_TARGET_LIBS Poco::NetSSLWin)
	endif()
endif()

target_link_libraries(CodeGen PUBLIC Poco::Foundation Poco::Util Poco::Net Poco::XSDTypes Poco::XSDParser ${ADD_TARGET_LIBS})

install(
	TARGETS CodeGen EXPORT "CodeGen"
	LIBRARY DESTINATION lib${LIB_SUFFIX}
	ARCHIVE DESTINATION lib${LIB_SUFFIX}
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
