add_executable(HTTPSTimeServer-win src/HTTPSTimeServer.cpp)
target_link_libraries(HTTPSTimeServer-win PUBLIC Poco::NetSSLWin Poco::Crypto Poco::XML)
