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

add_library(NetSSLWin ${SRCS})
add_library(Poco::NetSSLWin ALIAS NetSSLWin)
set_target_properties(NetSSLWin
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoNetSSLWin
	DEFINE_SYMBOL NetSSL_Win_EXPORTS
)

target_link_libraries(NetSSLWin PUBLIC Poco::Net Poco::Util Crypt32.lib)
target_include_directories(NetSSLWin
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(NetSSLWin)
POCO_GENERATE_PACKAGE(NetSSLWin)

if(ENABLE_SAMPLES)
	#TODO: Looks like the samples use crypto somehow?
	#add_subdirectory(samples)
endif()

if(ENABLE_TESTS)
	#add_subdirectory(testsuite)
endif()

