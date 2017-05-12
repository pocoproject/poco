set(SAMPLE_NAME "DNSSDHTTPTimeServer")

set(LOCAL_SRCS "")
aux_source_directory(src LOCAL_SRCS)

add_executable( ${SAMPLE_NAME} ${LOCAL_SRCS} )
target_link_libraries( ${SAMPLE_NAME} Foundation Net Util XML DNSSD ${DNSSD_IMPLEMENTATION_LIBRARY} )
