add_executable(tcpclient src/tcpclient.cpp)
target_link_libraries(tcpclient PUBLIC Poco::Net)
