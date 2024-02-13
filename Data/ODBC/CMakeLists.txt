# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(ODBC_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(ODBC_SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND ODBC_SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(DataODBC ${ODBC_SRCS})
add_library(Poco::DataODBC ALIAS DataODBC)
set_target_properties(DataODBC
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoDataODBC
	DEFINE_SYMBOL ODBC_EXPORTS
)

target_link_libraries(DataODBC PUBLIC Poco::Data ODBC::ODBC)
target_include_directories(DataODBC
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(DataODBC PUBLIC THREADSAFE)

POCO_INSTALL(DataODBC)
POCO_GENERATE_PACKAGE(DataODBC)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
