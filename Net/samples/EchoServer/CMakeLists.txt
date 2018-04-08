add_executable(EchoServer src/EchoServer.cpp)
target_link_libraries(EchoServer Poco::Net Poco::Util Poco::JSON Poco::XML Poco::Foundation)
