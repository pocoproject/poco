set(LIBNAME "mod_poco")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

include_directories( "include" )

add_library( ${LIBNAME} SHARED ${SRCS} )
set_target_properties( ${LIBNAME}
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    DEFINE_SYMBOL ApacheHandlers_EXPORTS)
target_link_libraries( ${LIBNAME} )

if (ENABLE_TESTS)
  add_subdirectory(samples)
endif ()