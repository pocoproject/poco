# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

# Version Resource
if(MSVC)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_executable(XSDGen ${SRCS})

target_link_libraries(XSDGen PRIVATE Poco::Foundation Poco::XML Poco::Util Poco::Net Poco::XSDTypes Poco::XSDParser)

# HTTPS schema retrieval
if(TARGET Poco::NetSSL)
	target_compile_definitions(XSDGen PRIVATE POCO_XSD_ENABLE_HTTPS)
	target_link_libraries(XSDGen PRIVATE Poco::NetSSL)
elseif(TARGET Poco::NetSSLWin)
	target_compile_definitions(XSDGen PRIVATE POCO_XSD_ENABLE_HTTPS)
	target_link_libraries(XSDGen PRIVATE Poco::NetSSLWin)
endif()

install(
	TARGETS XSDGen
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
