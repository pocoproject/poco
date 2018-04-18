# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

add_library(Crypto ${SRCS} )
add_library(Poco::Crypto ALIAS Crypto)
set_target_properties(Crypto
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME PocoCrypto
    DEFINE_SYMBOL Crypto_EXPORTS
    )

target_link_libraries(Crypto PUBLIC Poco::Foundation OpenSSL::SSL OpenSSL::Crypto)
target_include_directories(Crypto
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

if(MSVC AND POCO_DISABLE_INTERNAL_OPENSSL)
    target_compile_definitions(Crypto PUBLIC POCO_EXTERNAL_OPENSSL)	
endif()

if(MSVC AND NOT POCO_DISABLE_INTERNAL_OPENSSL)
	add_custom_command(TARGET Crypto PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${INTERNAL_OPENSSL_LIBRARY_DIR} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
		COMMENT "Copying OpenSSL binaries" VERBATIM)
endif(MSVC AND NOT POCO_DISABLE_INTERNAL_OPENSSL)

POCO_INSTALL(Crypto)
POCO_GENERATE_PACKAGE(Crypto)

if (POCO_ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
if (POCO_ENABLE_SAMPLES)
    add_subdirectory(samples)
endif ()
