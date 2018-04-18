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
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

POCO_INSTALL(Crypto)
POCO_GENERATE_PACKAGE(Crypto)

if (ENABLE_TESTS)
    add_subdirectory(samples)
    add_subdirectory(testsuite)
endif ()
