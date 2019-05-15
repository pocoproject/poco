# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

add_library(mod_poco ${SRCS})
set_target_properties(mod_poco
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    DEFINE_SYMBOL ApacheHandlers_EXPORTS)
target_include_directories(mod_poco
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${APACHE2_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions(mod_poco PRIVATE ${APACHE2_DEFINITIONS})
target_link_libraries(mod_poco PUBLIC Apache::Aprutil Apache::Apr Poco::Util Poco::Net)

if (POCO_ENABLE_SAMPLES)
  add_subdirectory(samples)
endif ()
