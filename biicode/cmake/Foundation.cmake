# If POCO_UNBUNDLED is enabled we try to find the required packages
# The configuration will fail if the packages are not found
if (POCO_UNBUNDLED)
    find_package(PCRE REQUIRED)
    set(SYSLIBS ${SYSLIBS} ${PCRE_LIBRARIES})
    target_include_directories(${BII_BLOCK_TARGET} INTERFACE ${PCRE_INCLUDE_DIRS})

    find_package(ZLIB REQUIRED)
    set(SYSLIBS ${SYSLIBS} ${ZLIB_LIBRARIES})
    target_include_directories(${BII_BLOCK_TARGET} INTERFACE ${ZLIB_INCLUDE_DIRS})
endif (POCO_UNBUNDLED)

if(WIN32)
    set(SYSLIBS ${SYSLIBS} iphlpapi)
endif(WIN32)
target_link_libraries(${BII_BLOCK_TARGET} INTERFACE ${SYSLIBS})
