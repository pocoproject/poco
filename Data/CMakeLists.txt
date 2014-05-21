set(LIBNAME "PocoData")

set(SRCS "")
aux_source_directory(src SRCS)
include_directories( SQLite/include )

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

add_subdirectory( SQLite )

if(MYSQL_FOUND)
    add_subdirectory( MySQL )
endif(MYSQL_FOUND)

include(FindODBC)

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
