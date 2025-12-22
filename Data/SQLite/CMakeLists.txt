# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SQLITE_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SQLITE_SRCS ${HDRS_G})

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SQLITE_SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(DataSQLite ${SQLITE_SRCS})
if (POCO_UNBUNDLED)
	target_link_libraries(DataSQLite PRIVATE SQLite::SQLite3)
	target_compile_definitions(DataSQLite PUBLIC POCO_SQLITE_UNBUNDLED)
else()
	target_link_libraries(DataSQLite PRIVATE "$<BUILD_LOCAL_INTERFACE:SQLite::SQLite3>")
endif()

add_library(Poco::DataSQLite ALIAS DataSQLite)
set_target_properties(DataSQLite
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoDataSQLite
	DEFINE_SYMBOL SQLite_EXPORTS
)
target_link_libraries(DataSQLite PUBLIC Poco::Data)
target_include_directories(DataSQLite
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(DataSQLite)
POCO_GENERATE_PACKAGE(DataSQLite)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
