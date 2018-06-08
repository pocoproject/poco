# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SQLITE_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SQLITE_SRCS ${HDRS_G})

if (POCO_UNBUNDLED)
	find_package(SQLite3 REQUIRED)
else()
    # sqlite3
    POCO_SOURCES( SQLITE_SRCS sqlite3
        src/sqlite3.c
    )

    POCO_HEADERS( SQLITE_SRCS sqlite3
        src/sqlite3.h
    )
endif()

add_library(SQLSQLite ${SQLITE_SRCS} )
add_library(Poco::SQLSQLite ALIAS SQLSQLite)
set_target_properties(SQLSQLite
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoSQLSQLite
    DEFINE_SYMBOL SQLite_EXPORTS
    )

target_link_libraries(SQLSQLite PUBLIC Poco::SQL ${SQLITE3_LIBRARIES}) # TODO(BJoe) Use namespace target
target_include_directories(SQLSQLite
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

if(POCO_UNBUNDLED)
	target_include_directories(SQLSQLite PUBLIC "${SQLITE3_INCLUDE_DIRS}")
	target_compile_definitions(SQLSQLite PUBLIC POCO_UNBUNDLED)
else()
	if(WINCE)
	    target_compile_definitions(SQLSQLite PRIVATE SQLITE_MSVC_LOCALTIME_API)
	endif(WINCE)
	target_compile_definitions(SQLSQLite PRIVATE 
	    SQLITE_THREADSAFE=1 
	    SQLITE_DISABLE_LFS
	    SQLITE_OMIT_UTF16
	    SQLITE_OMIT_PROGRESS_CALLBACK
	    SQLITE_OMIT_COMPLETE
	    SQLITE_OMIT_TCL_VARIABLE
	    SQLITE_OMIT_DEPRECATED
	)
endif()


POCO_INSTALL(SQLSQLite)
POCO_GENERATE_PACKAGE(SQLSQLite)

if (POCO_ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()

configure_file(PocoSQLSQLite.pc.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoSQLSQLite.pc @ONLY)
list(APPEND POCO_PKG_CONFIG_FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoSQLSQLite.pc)
set(POCO_PKG_CONFIG_FILES ${POCO_PKG_CONFIG_FILES} PARENT_SCOPE)
