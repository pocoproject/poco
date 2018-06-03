add_executable(Mail-ssl-win src/Mail.cpp)
target_link_libraries(Mail-ssl-win PUBLIC Poco::NetSSLWin Poco::Crypto Poco::XML)
