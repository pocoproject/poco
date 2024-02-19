# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})
if (NOT POCO_DATA_NO_SQL_PARSER)
	file(GLOB_RECURSE SRCS_PARSER "SQLParser/src/*.cpp")
	LIST(REMOVE_ITEM SRCS_PARSER "${CMAKE_CURRENT_SOURCE_DIR}/SQLParser/src/parser/conflict_test.cpp")
	POCO_SOURCES_AUTO(SRCS ${SRCS_PARSER})
endif()

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

if(MSVC AND NOT(MSVC_VERSION LESS 1400))
	set_source_files_properties(src/StatementImpl.cpp
		PROPERTIES COMPILE_FLAGS "/bigobj")
elseif(MINGW)
	set_source_files_properties(src/StatementImpl.cpp
		PROPERTIES COMPILE_FLAGS "-Wa,-mbig-obj")
endif()

# Version Resource
if(MSVC AND BUILD_SHARED_LIBS)
	source_group("Resources" FILES ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
	list(APPEND SRCS ${PROJECT_SOURCE_DIR}/DLLVersion.rc)
endif()

add_library(Data ${SRCS})
add_library(Poco::Data ALIAS Data)
set_target_properties(Data
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoData
	DEFINE_SYMBOL Data_EXPORTS
)

if (NOT POCO_DATA_NO_SQL_PARSER)
	target_compile_definitions(Data PUBLIC -DSQLParser_EXPORTS)
endif()
target_link_libraries(Data PUBLIC Poco::Foundation)

if(NOT POCO_DATA_NO_SQL_PARSER)
	target_include_directories(Data
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SQLParser/src
		PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SQLParser
	)
else()
	target_include_directories(Data
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)
endif()

POCO_INSTALL(Data)
POCO_GENERATE_PACKAGE(Data)

if(ENABLE_TESTS)
	add_subdirectory(DataTest)
	add_subdirectory(testsuite)
endif()

if(ENABLE_DATA_SQLITE)
	# SQlite3 is built in any case
	message(STATUS "SQLite Support Enabled")
	add_subdirectory(SQLite)
else(ENABLE_DATA_SQLITE)
	message(STATUS "SQLite Support Disabled")
endif()

if(MYSQL_FOUND AND ENABLE_DATA_MYSQL)
	message(STATUS "MySQL Support Enabled")
	add_subdirectory(MySQL)
else()
	message(STATUS "MySQL Support Disabled")
endif()

if(POSTGRESQL_FOUND AND ENABLE_DATA_POSTGRESQL)
	message(STATUS "PostgreSQL Support Enabled")
	add_subdirectory(PostgreSQL)
else()
	message(STATUS "PostgreSQL Support Disabled")
endif()

if(ODBC_FOUND AND ENABLE_DATA_ODBC)
	message(STATUS "ODBC Support Enabled")
	add_subdirectory(ODBC)
else()
	message(STATUS "ODBC Support Disabled")
endif()

if(ENABLE_SAMPLES)
	add_subdirectory(samples)
endif()
