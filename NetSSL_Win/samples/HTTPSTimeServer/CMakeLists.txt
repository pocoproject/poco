add_executable(HTTPSTimeServer-win src/HTTPSTimeServer.cpp)
target_link_libraries(HTTPSTimeServer-win Poco::NetSSLWin Poco::Crypto Poco::XML)
