add_executable(WebSocketServer src/WebSocketServer.cpp)

target_link_libraries(WebSocketServer PUBLIC Poco::Net Poco::Util Poco::JSON)
