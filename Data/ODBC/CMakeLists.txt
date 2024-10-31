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

# Search for MS ODBC header file which is a pre-requisite to enable
# ENABLE_DATA_SQL_SERVER_BIG_STRINGS
find_file(
	_msodbc_h

	msodbcsql.h

	HINTS

	/usr/include
	#macOS
	/opt/homebrew/include/msodbcsql18
	/opt/homebrew/include/msodbcsql17
	#Linux
	/opt/microsoft/msodbcsql18/include
	/opt/microsoft/msodbcsql17/include/
	/opt/microsoft/msodbcsql/include/
	# Windows?

	NO_CACHE
)

if (_msodbc_h)
	get_filename_component(MSODBC_DIR ${_msodbc_h} DIRECTORY CACHE)
	message(STATUS "ODBC: Found msodbcsql.h in: ${MSODBC_DIR}")
	target_include_directories(DataODBC PUBLIC "${MSODBC_DIR}")

	if (ENABLE_DATA_SQL_SERVER_BIG_STRINGS)
		target_compile_definitions(DataODBC PUBLIC POCO_DATA_SQL_SERVER_BIG_STRINGS=1)
	else()
		target_compile_definitions(DataODBC PUBLIC POCO_DATA_SQL_SERVER_BIG_STRINGS=0)
	endif()
else()
	# Disable MS SQL specific setting when msodbcsql.h is not present
	set(ENABLE_DATA_SQL_SERVER_BIG_STRINGS OFF CACHE BOOL FORCE "Enable MS SQL Server big strings")
endif()
unset(_msodbc_h)

POCO_INSTALL(DataODBC)
POCO_GENERATE_PACKAGE(DataODBC)

if(ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
