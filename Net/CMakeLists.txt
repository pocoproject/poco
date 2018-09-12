# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

add_library(Net ${SRCS} )
add_library(Poco::Net ALIAS Net)
set_target_properties( Net
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
	OUTPUT_NAME PocoNet
    DEFINE_SYMBOL Net_EXPORTS
    )

target_link_libraries(Net PUBLIC Poco::Foundation)
# Windows and WindowsCE need additional libraries
if(WIN32)
    target_link_libraries(Net PUBLIC "iphlpapi")
    if(WINCE)
        target_link_libraries(Net PUBLIC "ws2")
    else()    
        target_link_libraries(Net PUBLIC "ws2_32")
    endif()

    if(MINGW) # Needed by WinAPI in MinGW
        string(REGEX MATCH "^[0-9]+\\.[0-9]+" WIN_VERSION ${CMAKE_SYSTEM_VERSION})
        string(REGEX REPLACE "\\.[0-9]+$" "" WIN_MAJOR_VERSION ${WIN_VERSION})
        string(REGEX REPLACE "^[0-9]+\\." "" WIN_MINOR_VERSION ${WIN_VERSION})
        math(EXPR WINAPI_VERSION "256 * ${WIN_MAJOR_VERSION} + ${WIN_MINOR_VERSION}")
        set_source_files_properties(src/PollSet.cpp
                PROPERTIES COMPILE_DEFINITIONS "_WIN32_WINNT=${WINAPI_VERSION}")
    endif()
endif(WIN32)

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

configure_file(PocoNet.pc.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoNet.pc @ONLY)
list(APPEND POCO_PKG_CONFIG_FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/PocoNet.pc)
set(POCO_PKG_CONFIG_FILES ${POCO_PKG_CONFIG_FILES} PARENT_SCOPE)
