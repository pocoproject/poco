set(LIBNAME "PocoDataODBC")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( ODBC_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( ODBC_SRCS ${HDRS_G})

add_definitions( ${ODBC_CFLAGS} )

include_directories( "include" )

add_library( ${LIBNAME} ${LIB_MODE} ${ODBC_SRCS} )
set_target_properties( ${LIBNAME} 
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} 
    DEFINE_SYMBOL ODBC_EXPORTS)
target_link_libraries( ${LIBNAME} PocoData PocoFoundation ${ODBC_LIBRARIES})

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

if (ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
