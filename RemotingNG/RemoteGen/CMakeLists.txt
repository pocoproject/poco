# Sources
file(GLOB SRCS_GEN_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS_GEN ${SRCS_GEN_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(SRCS_GEN ${HDRS_G})

# Version Resource
if(MSVC)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/ProDLLVersion.rc)
	list(APPEND SRCS_GEN ${PROJECT_SOURCE_DIR}/ProDLLVersion.rc)
endif()

add_executable(RemoteGenNG ${SRCS_GEN})
set_target_properties(RemoteGenNG
	PROPERTIES
	OUTPUT_NAME RemoteGenNG
)

configure_file(RemoteGenNG.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)

target_link_libraries(RemoteGenNG
	PUBLIC
	Poco::Foundation Poco::Util Poco::XML Poco::JSON
	Poco::RemotingNG Poco::CodeGeneration Poco::CppParser
)

if(WIN32)
	set(PocoConfigPackageLocation "cmake")
else()
	set(PocoConfigPackageLocation "${CMAKE_INSTALL_LIBDIR}/")
endif()

install(
	TARGETS RemoteGenNG EXPORT RemoteGenNGTargets
	LIBRARY DESTINATION lib${LIB_SUFFIX}
	ARCHIVE DESTINATION lib${LIB_SUFFIX}
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)

install(
	FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RemoteGenNG.xml
	TYPE BIN
)
