# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( WIN_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( WIN_SRCS ${HDRS_G})

file(GLOB_RECURSE HDRS_G "src/*.h" )
POCO_HEADERS_AUTO( WIN_SRCS ${HDRS_G})

add_library(WinTestRunner ${WIN_SRCS} )
set_target_properties(WinTestRunner
    PROPERTIES
    VERSION "1" SOVERSION "1"
    DEFINE_SYMBOL WinTestRunner_EXPORTS)
target_link_libraries(WinTestRunner PUBLIC Poco::CppUnit)
target_include_directories(WinTestRunner
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
# TODO: Is this flag _AFXDLL always required?
target_compile_definitions(WinTestRunner
  PRIVATE
  _AFXDLL
  )
#TODO: Use instead of the flag _AFXDLL above: find_package(MFC)


if(WIN32)
	target_link_libraries(WinTestRunner PUBLIC winmm)
endif(WIN32)
