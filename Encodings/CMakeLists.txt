# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES( SRCS Encodings ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS( SRCS Encodings ${HDRS_G})

add_library(Encodings ${LIB_MODE} ${SRCS} )
add_library(Poco::Encodings ALIAS Encodings)
set_target_properties(Encodings
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME PocoEncodings
    DEFINE_SYMBOL Encodings_EXPORTS
    )

target_link_libraries(Encodings PUBLIC Poco::Foundation)
target_include_directories(Encodings
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

POCO_INSTALL(Encodings)
POCO_GENERATE_PACKAGE(Encodings)

if(POCO_ENABLE_TESTS)
	add_subdirectory(testsuite)
endif()
if(POCO_ENABLE_SAMPLES)
    add_subdirectory(samples)
endif()

configure_file(PocoEncodings.pc.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoEncodings.pc @ONLY)
list(APPEND POCO_PKG_CONFIG_FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoEncodings.pc)
set(POCO_PKG_CONFIG_FILES ${POCO_PKG_CONFIG_FILES} PARENT_SCOPE)
