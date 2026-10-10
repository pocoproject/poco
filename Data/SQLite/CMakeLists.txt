# Sources
file(GLOB SRCS_G "src/*.cpp")
if(POCO_DATA_NO_SQL_PARSER)
	list(REMOVE_ITEM SRCS_G "${CMAKE_CURRENT_SOURCE_DIR}/src/MemoryDB.cpp")
	list(REMOVE_ITEM SRCS_G "${CMAKE_CURRENT_SOURCE_DIR}/src/MemoryDBInspector.cpp")
endif()
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

if(ENABLE_DATA_SQLITE_VEC)
	target_link_libraries(DataSQLite PRIVATE "$<BUILD_LOCAL_INTERFACE:SQLiteVec::SQLiteVec>")
	target_compile_definitions(DataSQLite PUBLIC POCO_ENABLE_SQLITE_VEC)
endif()

# The bundled sqlite3 object library carries SQLITE_ENABLE_FTS5 PUBLIC, but is
# linked through $<BUILD_LOCAL_INTERFACE:...> PRIVATE, so the define would never
# reach consumers (e.g. the testsuite's testFTS guard). Re-export it here, like
# POCO_ENABLE_SQLITE_VEC above, so consumers can detect FTS5 availability.
if(ENABLE_DATA_SQLITE_FTS5 AND NOT POCO_SQLITE_UNBUNDLED)
	target_compile_definitions(DataSQLite PUBLIC SQLITE_ENABLE_FTS5)
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

# MemoryDB.cpp #includes "SQLParser.h" / "sql/..." headers from the bundled
# parser. Mirror Data/CMakeLists.txt's PRIVATE include of Data/SQLParser so
# the cmake build resolves them the same way the gnu-make build does (via
# Data/SQLite/Makefile's target_includes under the same SQL-parser guard).
if(NOT POCO_DATA_NO_SQL_PARSER)
	target_include_directories(DataSQLite
		PRIVATE
			${PROJECT_SOURCE_DIR}/Data/SQLParser/src
			${PROJECT_SOURCE_DIR}/Data/SQLParser
	)
endif()

POCO_INSTALL(DataSQLite)
POCO_GENERATE_PACKAGE(DataSQLite)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
