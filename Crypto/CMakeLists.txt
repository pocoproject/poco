set(LIBNAME "Crypto")
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
    DEFINE_SYMBOL Crypto_EXPORTS
    )

target_link_libraries( "${LIBNAME}" Foundation OpenSSL::SSL OpenSSL::Crypto )
target_include_directories( "${LIBNAME}"
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions("${LIBNAME}" PUBLIC ${LIB_MODE_DEFINITIONS})

if(MSVC AND NOT USING_INTERNAL_OPENSSL)
    target_compile_definitions("${LIBNAME}" PUBLIC POCO_EXTERNAL_OPENSSL)	
endif()

if(MSVC AND USING_INTERNAL_OPENSSL)
	add_custom_command(TARGET "${LIBNAME}" PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${INTERNAL_OPENSSL_LIBRARY_DIR} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
		COMMENT "Copying OpenSSL binaries" VERBATIM)
endif(MSVC AND USING_INTERNAL_OPENSSL)

POCO_INSTALL("${LIBNAME}")
POCO_GENERATE_PACKAGE("${LIBNAME}")

if (ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
if (ENABLE_SAMPLES)
    add_subdirectory(samples)
endif ()
