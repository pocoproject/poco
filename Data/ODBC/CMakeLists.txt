set(LIBNAME "DataODBC")
set(POCO_LIBNAME "Poco${LIBNAME}")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( ODBC_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( ODBC_SRCS ${HDRS_G})

add_definitions( ${ODBC_CFLAGS} -DTHREADSAFE)

add_library( "${LIBNAME}" ${LIB_MODE} ${ODBC_SRCS} )
add_library( "${POCO_LIBNAME}" ALIAS "${LIBNAME}")
set_target_properties( "${LIBNAME}"
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME ${POCO_LIBNAME}
    DEFINE_SYMBOL ODBC_EXPORTS
    )

target_link_libraries( "${LIBNAME}" Foundation Data ${ODBC_LIBRARIES})
target_include_directories( "${LIBNAME}"
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
install(
    DIRECTORY include/Poco
    DESTINATION include
    COMPONENT Devel
    PATTERN ".svn" EXCLUDE
    )

install(
    TARGETS "${LIBNAME}" EXPORT "${LIBNAME}Targets"
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    )

POCO_GENERATE_PACKAGE("${LIBNAME}" "${LIBNAME}Targets" "lib/cmake/${PROJECT_NAME}")

if (ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
