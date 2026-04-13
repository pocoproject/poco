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

add_library(SSH ${SRCS})
add_library(Poco::SSH ALIAS SSH)
set_target_properties(SSH
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoSSH
	DEFINE_SYMBOL SSH_EXPORTS
)

target_link_libraries(SSH PUBLIC Poco::Foundation)
target_link_libraries(SSH PUBLIC ${LIBSSH_LIBRARIES})
target_include_directories(SSH
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
		${LIBSSH_INCLUDE_DIRS}
)

POCO_INSTALL(SSH)
POCO_GENERATE_PACKAGE(SSH)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()
