add_executable(Mail-ssl-win src/Mail.cpp)
target_link_libraries(Mail-ssl-win Poco::NetSSLWin Poco::Crypto Poco::Util Poco::Net Poco::XML Poco::Foundation)
