# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

add_library(CppUnit ${SRCS})
set_target_properties(CppUnit
    PROPERTIES
    VERSION "1" SOVERSION "1"
    DEFINE_SYMBOL CppUnit_EXPORTS)
target_link_libraries(CppUnit Poco::Foundation)
target_include_directories(CppUnit
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions(CppUnit PUBLIC POCO_NO_AUTOMATIC_LIBS)
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(CppUnit
        PUBLIC
            POCO_STATIC
    )
endif()
if (WIN32)
    target_compile_definitions(CppUnit PRIVATE _CRT_SECURE_NO_WARNINGS)
endif (WIN32)
