add_executable(HTTPSTimeServer src/HTTPSTimeServer.cpp)
target_link_libraries(HTTPSTimeServer Poco::NetSSL Poco::Crypto Poco::Util Poco::Net Poco::XML Poco::Foundation)
