# If POCO_UNBUNDLED is enabled we try to find the required packages
# The configuration will fail if the packages are not found
if (POCO_UNBUNDLED)
    find_package(EXPAT REQUIRED)
    set(SYSLIBS ${SYSLIBS} ${EXPAT_LIBRARIES})
    include_directories(${EXPAT_INCLUDE_DIRS})
endif (POCO_UNBUNDLED)

if(WIN32)
    #TODO: Is XML_STATIC only required with Windows? What does it do?
    target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE -DXML_STATIC -DXML_NS -DXML_DTD -DHAVE_EXPAT_CONFIG_H)
else()
    target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE -DXML_NS -DXML_DTD -DHAVE_EXPAT_CONFIG_H)
endif()
target_link_libraries(${BII_BLOCK_TARGET} INTERFACE ${SYSLIBS})
