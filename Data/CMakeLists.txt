# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SRCS ${HDRS_G})

if(MSVC AND NOT(MSVC_VERSION LESS 1400))
	set_source_files_properties(src/StatementImpl.cpp
		PROPERTIES COMPILE_FLAGS "/bigobj")
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

target_link_libraries(Data PUBLIC Poco::Foundation)
target_include_directories(Data
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

POCO_INSTALL(Data)
POCO_GENERATE_PACKAGE(Data)

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

if(ODBC_FOUND AND ENABLE_DATA_ODBC AND NOT WINCE)
	message(STATUS "ODBC Support Enabled")
	add_subdirectory(ODBC)
else()
	message(STATUS "ODBC Support Disabled")
endif()

if(ENABLE_TESTS)
	add_subdirectory(samples)
	add_subdirectory(testsuite)
endif()
