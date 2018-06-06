# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

if(MSVC AND NOT(MSVC_VERSION LESS 1400))
    set_source_files_properties(src/StatementImpl.cpp
        PROPERTIES COMPILE_FLAGS "/bigobj")
endif()


add_library(Data ${SRCS} )
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
    add_subdirectory( SQLite )
else(ENABLE_DATA_SQLITE)
    message(STATUS "SQLite Support Disabled")
endif()

if(MYSQL_FOUND AND ENABLE_DATA_MYSQL)
    include_directories("${MYSQL_INCLUDE_DIR}") # TODO(Bjoe) use target_include_directories or target_link_libraries
    message(STATUS "MySQL Support Enabled")
    add_subdirectory( MySQL )
else()
    message(STATUS "MySQL Support Disabled - no MySQL library")
endif()

if(POSTGRESQL_FOUND AND ENABLE_DATA_POSTGRESQL)
    include_directories("${PostgreSQL_INCLUDE_DIR}") # TODO(Bjoe) Use target_link_libraries()
    message(STATUS "PostgreSQL Support Enabled")
    add_subdirectory( PostgreSQL )
else()
    message(STATUS "PostgreSQL Support Disabled - no PostgreSQL library")
endif()

if(WIN32 AND NOT WINCE AND ENABLE_DATA_ODBC)
    set(ODBC_LIBRARIES "odbc32" "odbccp32") # TODO(Bjoe) Verify if this is not set via find_package(ODBC)
    message(STATUS "Windows native ODBC Support Enabled")
    add_subdirectory( ODBC )
else()
    if(ODBC_FOUND AND ENABLE_DATA_ODBC)
	include_directories("${ODBC_INCLUDE_DIRECTORIES}") # TODO(Bjoe) use target_include_directories or target_link_libraries
	message(STATUS "ODBC Support Enabled")
	add_subdirectory( ODBC )
    else()
	message(STATUS "ODBC Support Disabled - no ODBC runtime")
    endif()
endif()

if (ENABLE_TESTS)
    add_subdirectory(samples)
    add_subdirectory(testsuite)
endif ()
