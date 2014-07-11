set(LIBNAME "PocoData")

set(SRCS
    src/AbstractBinder.cpp
    src/AbstractBinding.cpp
    src/AbstractExtraction.cpp
    src/AbstractExtractor.cpp
    src/AbstractPreparation.cpp
    src/AbstractPreparator.cpp
    src/ArchiveStrategy.cpp
    src/Bulk.cpp
    src/Connector.cpp
    src/DataException.cpp
    src/Date.cpp
    src/DynamicLOB.cpp
    src/Limit.cpp
    src/MetaColumn.cpp
    src/PooledSessionHolder.cpp
    src/PooledSessionImpl.cpp
    src/Position.cpp
    src/Range.cpp
    src/RecordSet.cpp
    src/Row.cpp
    src/RowFilter.cpp
    src/RowFormatter.cpp
    src/RowIterator.cpp
    src/Session.cpp
    src/SessionFactory.cpp
    src/SessionImpl.cpp
    src/SessionPoolContainer.cpp
    src/SessionPool.cpp
    src/SimpleRowFormatter.cpp
    src/SQLChannel.cpp
    src/Statement.cpp
    src/StatementCreator.cpp
    src/StatementImpl.cpp
    src/Time.cpp
    src/Transaction.cpp
)


if (NOT POCO_STATIC)
    add_definitions(-DData_EXPORTS -DTHREADSAFE -DODBC_EXPORTS -DMySQL_EXPORTS -DSQLite_EXPORTS)
endif (NOT POCO_STATIC)

if(MSVC AND NOT(MSVC_VERSION LESS 1400))
    set_source_files_properties(src/StatementImpl.cpp
        PROPERTIES COMPILE_FLAGS "/bigobj")
endif()

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME}
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
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
    add_subdirectory( MySQL )
endif(MYSQL_FOUND)

find_package(ODBC)
if(CMAKE_SYSTEM MATCHES "Windows")
    message(STATUS "Windows native ODBC Support Enabled")
    add_subdirectory( ODBC )
else ()
    if ( ${UNIX_ODBC_CONFIG} STREQUAL UNIX_ODBC_CONFIG-NOTFOUND AND ${IODBC_CONFIG} STREQUAL IODBC_CONFIG-NOTFOUND)
        message(STATUS "ODBC Support Disabled - no ODBC runtime")
    else ()
        message(STATUS "ODBC Support Enabled")
        add_subdirectory( ODBC )
    endif ()
endif()

if (ENABLE_TESTS)
    add_subdirectory(samples)
    add_subdirectory(testsuite)
endif ()
