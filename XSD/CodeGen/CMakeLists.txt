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

if(ENABLE_TESTS)
	# XSDGen rejects schema values that would end a //@ comment line in the generated code.
	# Poco::Util::Application takes options in the /name=value form on Windows.
	if(WIN32)
		set(XSDGEN_CONFIG_OPTION "/config-file=${CMAKE_CURRENT_SOURCE_DIR}/testsuite/data/XSDGen.xml")
	else()
		set(XSDGEN_CONFIG_OPTION "--config-file=${CMAKE_CURRENT_SOURCE_DIR}/testsuite/data/XSDGen.xml")
	endif()
	add_test(
		NAME XSDGen-ElementNameInjection
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND XSDGen ${XSDGEN_CONFIG_OPTION} ${CMAKE_CURRENT_SOURCE_DIR}/testsuite/data/ElementNameInjection.xsd
	)
	add_test(
		NAME XSDGen-SoapActionInjection
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND XSDGen ${XSDGEN_CONFIG_OPTION} ${CMAKE_CURRENT_SOURCE_DIR}/testsuite/data/SoapActionInjection.wsdl
	)
	set_tests_properties(XSDGen-ElementNameInjection XSDGen-SoapActionInjection PROPERTIES
		PASS_REGULAR_EXPRESSION "contains a character that is not allowed in a Remoting attribute")
endif()
