set(LIBNAME "CppUnit")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME}
    PROPERTIES
    VERSION "1" SOVERSION "1"
    DEFINE_SYMBOL CppUnit_EXPORTS)
target_link_libraries( ${LIBNAME} )

if(WIN32)
    add_subdirectory(WinTestRunner)
endif(WIN32)
