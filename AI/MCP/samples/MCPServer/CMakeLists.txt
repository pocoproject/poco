poco_add_executable(MCPServer src/MCPServer.cpp)
target_link_libraries(MCPServer PUBLIC Poco::AIMCP Poco::Util)
