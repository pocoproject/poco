set(LIBNAME "WinTestRunner")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( WIN_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( WIN_SRCS ${HDRS_G})

file(GLOB_RECURSE HDRS_G "src/*.h" )
POCO_HEADERS_AUTO( WIN_SRCS ${HDRS_G})

# TODO: Is this flag always required?
add_definitions("-D_AFXDLL")

include_directories( "src" )

add_library( ${LIBNAME} ${LIB_MODE} ${WIN_SRCS} )
set_target_properties( ${LIBNAME}
    PROPERTIES
    VERSION "1" SOVERSION "1"
    DEFINE_SYMBOL WinTestRunner_EXPORTS)
target_link_libraries( ${LIBNAME} CppUnit )
if(WIN32)
    target_link_libraries( ${LIBNAME} winmm )
endif(WIN32)


