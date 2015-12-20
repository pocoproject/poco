set(LIBNAME "NetSSLWin")
set(POCO_LIBNAME "Poco${LIBNAME}")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

add_library( "${LIBNAME}" ${LIB_MODE} ${SRCS} )
add_library( "${POCO_LIBNAME}" ALIAS "${LIBNAME}")
set_target_properties( "${LIBNAME}"
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME ${POCO_LIBNAME}
    DEFINE_SYMBOL NetSSL_Win_EXPORTS
    )

target_link_libraries( "${LIBNAME}" Net Util Foundation Crypt32.lib )
target_include_directories( "${LIBNAME}"
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions("${LIBNAME}" PUBLIC ${LIB_MODE_DEFINITIONS})

POCO_INSTALL("${LIBNAME}")
POCO_GENERATE_PACKAGE("${LIBNAME}")

if (ENABLE_TESTS)
if (WIN32)
#   FIXME: 
#	add_subdirectory(testsuite)
    message(STATUS "############ FIXME: ${LIBNAME} add_subdirectory(testsuite)")
else ()
    add_subdirectory(testsuite)
endif ()
endif ()
if (ENABLE_SAMPLES)
if (WIN32)
#   FIXME: Looks like the samples use crypto somehow?
#   add_subdirectory(samples)
    message(STATUS "############ FIXME: ${LIBNAME} add_subdirectory(samples)")
else ()
    add_subdirectory(testsuite)
endif ()
endif ()

