# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

# Windows and WindowsCE need additional libraries
if(WIN32)
    if(WINCE)
        set(SYSLIBS ${SYSLIBS} "ws2.lib" "iphlpapi.lib")
    else()
        set(SYSLIBS ${SYSLIBS} "ws2_32.lib" "iphlpapi.lib")
    endif()
endif(WIN32)

add_library(Net ${SRCS} )
add_library(Poco::Net ALIAS Net)
set_target_properties( Net
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoNet
    DEFINE_SYMBOL Net_EXPORTS
    )

target_link_libraries(Net PUBLIC Poco::Foundation ${SYSLIBS})
target_include_directories(Net
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

POCO_INSTALL(Net)
POCO_GENERATE_PACKAGE(Net)

if (POCO_ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
if (POCO_ENABLE_SAMPLES)
    add_subdirectory(samples)
endif ()
