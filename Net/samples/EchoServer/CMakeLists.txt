add_executable(EchoServer src/EchoServer.cpp)
target_link_libraries(EchoServer PUBLIC Poco::Net Poco::Util Poco::JSON)
