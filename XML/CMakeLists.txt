set(LIBNAME "PocoXML")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h" )
POCO_HEADERS_AUTO( SRCS ${HDRS_G})

# If POCO_UNBUNDLED is enabled we try to find the required packages
# The configuration will fail if the packages are not found
if (POCO_UNBUNDLED)
    find_package(EXPAT REQUIRED)
    set(SYSLIBS ${SYSLIBS} ${EXPAT_LIBRARIES})
    include_directories(${EXPAT_INCLUDE_DIRS})
else()
    POCO_SOURCES( SRCS expat
    src/xmlparse.cpp
    src/xmlrole.c
    src/xmltok.c
    src/xmltok_impl.c
    src/xmltok_ns.c
    )
endif (POCO_UNBUNDLED)

add_definitions(-D_USRDLL -DXML_NS -DXML_DTD -DHAVE_EXPAT_CONFIG_H)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME}
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION}
    DEFINE_SYMBOL XML_EXPORTS)
target_link_libraries( ${LIBNAME} ${SYSLIBS} PocoFoundation)

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )

install(
    TARGETS ${LIBNAME}
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin
    )

if (ENABLE_TESTS)
  add_subdirectory(samples)
  add_subdirectory(testsuite)
endif ()

