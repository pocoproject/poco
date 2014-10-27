set(TESTUNIT "${LIBNAME}-testrunner")

# Sources
file(GLOB SRCS_G "src/*.cpp")
file(GLOB SRCS_G_REMOVE
    src/WinDriver.cpp
)
list(REMOVE_ITEM SRCS_G ${SRCS_G_REMOVE})
POCO_SOURCES_AUTO( TEST_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h" )
POCO_HEADERS_AUTO( TEST_SRCS ${HDRS_G})

if(ENABLE_WINTESTRUNNER)
    POCO_SOURCES_AUTO_PLAT( TEST_SRCS WIN32
        src/WinDriver.cpp
    )
endif(ENABLE_WINTESTRUNNER)

add_executable( ${TESTUNIT} ${TEST_SRCS} )
add_test(NAME ${LIBNAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${TESTUNIT} -all)
target_link_libraries( ${TESTUNIT} PocoNet PocoUtil PocoXML PocoFoundation CppUnit)
if( WIN32)
    if(ENABLE_WINTESTRUNNER)
        add_definitions("-D_AFXDLL")
        target_link_libraries( ${TESTUNIT} WinTestRunner)
    endif(ENABLE_WINTESTRUNNER)
endif(WIN32)
