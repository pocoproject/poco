# ReplicaSet sample - demonstrates replica set features
set(SAMPLE_NAME "ReplicaSet")
set(SRCS src/ReplicaSet.cpp)
add_executable(${SAMPLE_NAME} ${SRCS})
target_link_libraries(${SAMPLE_NAME} Poco::MongoDB Poco::Net Poco::Foundation)

# ReplicaSetMonitor sample - monitoring and health check tool
set(MONITOR_NAME "ReplicaSetMonitor")
set(MONITOR_SRCS src/ReplicaSetMonitor.cpp)
add_executable(${MONITOR_NAME} ${MONITOR_SRCS})
target_link_libraries(${MONITOR_NAME} Poco::MongoDB Poco::Net Poco::Foundation)

# URIExample sample - demonstrates URI parsing
set(URI_NAME "URIExample")
set(URI_SRCS src/URIExample.cpp)
add_executable(${URI_NAME} ${URI_SRCS})
target_link_libraries(${URI_NAME} Poco::MongoDB Poco::Net Poco::Foundation)
