add_executable(tcpserver src/tcpserver.cpp)
target_link_libraries(tcpserver PUBLIC Poco::Net)
