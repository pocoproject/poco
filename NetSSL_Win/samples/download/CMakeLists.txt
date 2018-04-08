add_executable(download-ssl-win src/download.cpp)
target_link_libraries(download-ssl-win Poco::NetSSLWin Poco::Crypto Poco::Util Poco::Net Poco::XML Poco::Foundation)
