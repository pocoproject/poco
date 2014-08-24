set(LIBNAME "PocoData")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

if (NOT POCO_STATIC)
    add_definitions(-DTHREADSAFE)
endif (NOT POCO_STATIC)

if(MSVC AND NOT(MSVC_VERSION LESS 1400))
    set_source_files_properties(src/StatementImpl.cpp
        PROPERTIES COMPILE_FLAGS "/bigobj")
endif()

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME}
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    DEFINE_SYMBOL Data_EXPORTS)
target_link_libraries( ${LIBNAME} PocoFoundation)

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )

install(
    TARGETS ${LIBNAME}
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin
    )

# SQlite3 is built in any case
add_subdirectory( SQLite )

find_package(MySQL)
if(MYSQL_FOUND)
    include_directories("${MYSQL_INCLUDE_DIR}")
    message(STATUS "MySQL Support Enabled")
    add_subdirectory( MySQL )
else()
    message(STATUS "MySQL Support Disabled - no MySQL library")
endif(MYSQL_FOUND)

find_package(ODBC)
if(CMAKE_SYSTEM MATCHES "Windows")
    set(ODBC_LIBRARIES "")
    message(STATUS "Windows native ODBC Support Enabled")
    add_subdirectory( ODBC )
else ()
    if (ODBC_FOUND)
        include_directories("${ODBC_INCLUDE_DIRECTORIES}")
        message(STATUS "ODBC Support Enabled")
        add_subdirectory( ODBC )
    else ()
        message(STATUS "ODBC Support Disabled - no ODBC runtime")
    endif ()
endif()

if (ENABLE_TESTS)
    add_subdirectory(samples)
    add_subdirectory(testsuite)
endif ()
