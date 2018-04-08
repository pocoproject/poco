# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

add_library(NetSSL ${LIB_MODE} ${SRCS} )
add_library(Poco::NetSSL ALIAS NetSSL)
set_target_properties( NetSSL
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoNetSSL
    DEFINE_SYMBOL NetSSL_EXPORTS
    )

target_link_libraries(NetSSL PUBLIC Poco::Crypto Poco::Net Poco::Util Poco::Foundation OpenSSL::Crypto OpenSSL::SSL)
target_include_directories(NetSSL
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions(NetSSL PUBLIC ${LIB_MODE_DEFINITIONS})

POCO_INSTALL(NetSSL)
POCO_GENERATE_PACKAGE(NetSSL)

if (ENABLE_TESTS)
    add_subdirectory(samples)
    add_subdirectory(testsuite)
endif ()

