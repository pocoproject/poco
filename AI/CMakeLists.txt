# The AI library family: the Model Context Protocol core (MCP), its HTTP
# client transport (MCP/HTTP) and the AI library itself, which depends on
# both and is therefore configured after them.
if(ENABLE_AI_MCP)
	add_subdirectory(MCP)
endif()

if(ENABLE_AI)
	# Sources
	file(GLOB SRCS_G "src/*.cpp")
	POCO_SOURCES_AUTO(SRCS ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "include/*.h")
	POCO_HEADERS_AUTO(SRCS ${HDRS_G})

	# Version Resource
	if(MSVC AND BUILD_SHARED_LIBS)
		source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
		list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	endif()

	add_library(AI ${SRCS})
	add_library(Poco::AI ALIAS AI)
	set_target_properties(AI
		PROPERTIES
		VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
		OUTPUT_NAME PocoAI
		DEFINE_SYMBOL AI_EXPORTS
	)

	# Net only: HTTPS comes from Poco::Net::HTTPSessionFactory and the
	# instantiator the application registers, never from a NetSSL link.
	target_link_libraries(AI PUBLIC Poco::AIMCPHTTP Poco::AIMCP Poco::Net Poco::JSON)
	target_include_directories(AI
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
	)

	POCO_INSTALL(AI)
	POCO_GENERATE_PACKAGE(AI)

	if(ENABLE_TESTS)
		add_subdirectory(testsuite)
	endif()
endif()
