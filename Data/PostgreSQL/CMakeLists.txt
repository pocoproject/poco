# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(POSTGRESQL_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(POSTGRESQL_SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND POSTGRESQL_SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(DataPostgreSQL ${POSTGRESQL_SRCS})
add_library(Poco::DataPostgreSQL ALIAS DataPostgreSQL)
set_target_properties(DataPostgreSQL
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoDataPostgreSQL
	DEFINE_SYMBOL PostgreSQL_EXPORTS
)

target_link_libraries(DataPostgreSQL PUBLIC Poco::Data PostgreSQL::client)
target_include_directories(DataPostgreSQL
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

POCO_INSTALL(DataPostgreSQL)
POCO_GENERATE_PACKAGE(DataPostgreSQL)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
