add_executable(HTTPSTimeServer src/HTTPSTimeServer.cpp)
target_link_libraries(HTTPSTimeServer Poco::NetSSL Poco::Util Poco::XML)
