# Sources
file(GLOB SRCS_GEN_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS_GEN ${SRCS_GEN_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(SRCS_GEN ${HDRS_G})

# Version Resource
if(MSVC)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS_GEN ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_executable(RemoteGenNG ${SRCS_GEN})
set_target_properties(RemoteGenNG
	PROPERTIES
	OUTPUT_NAME RemoteGenNG
)

configure_file(RemoteGenNG.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)

target_link_libraries(RemoteGenNG
	PUBLIC
	Poco::Foundation Poco::Util Poco::XML Poco::JSON Poco::Net
	Poco::RemotingNG Poco::CodeGeneration Poco::CppParser
)

install(
	TARGETS RemoteGenNG
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/RemoteGenNG.xml.install
	DESTINATION ${CMAKE_INSTALL_BINDIR}
	RENAME RemoteGenNG.xml
)
