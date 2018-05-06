# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( ODBC_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( ODBC_SRCS ${HDRS_G})

add_library(DataODBC ${ODBC_SRCS})
add_library(Poco::DataODBC ALIAS DataODBC)
set_target_properties(DataODBC
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME PocoDataODBC
    DEFINE_SYMBOL ODBC_EXPORTS
    )

target_link_libraries(DataODBC PUBLIC Poco::Data ${ODBC_LIBRARIES})
target_include_directories(DataODBC
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
target_compile_definitions(DataODBC PUBLIC ${ODBC_CFLAGS} THREADSAFE)

POCO_INSTALL(DataODBC)
POCO_GENERATE_PACKAGE(DataODBC)

if (ENABLE_TESTS)
    add_subdirectory(testsuite)
endif ()
