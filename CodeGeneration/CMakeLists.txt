# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/ProDLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/ProDLLVersion.rc)
endif()

add_library(CodeGeneration ${SRCS})
add_library(Poco::CodeGeneration ALIAS CodeGeneration)
set_target_properties(CodeGeneration
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoCodeGeneration
	DEFINE_SYMBOL CodeGeneration_EXPORTS
)

target_link_libraries(CodeGeneration PUBLIC Poco::CppParser Poco::Foundation)
target_include_directories(CodeGeneration
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(CodeGeneration)
POCO_GENERATE_PACKAGE(CodeGeneration)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
