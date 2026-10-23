poco_add_executable(MCPHTTPServer src/MCPHTTPServer.cpp)
target_link_libraries(MCPHTTPServer PUBLIC Poco::AIMCPHTTP Poco::Util)
