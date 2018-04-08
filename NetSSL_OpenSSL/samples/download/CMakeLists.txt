add_executable(download-ssl src/download.cpp)
target_link_libraries(download-ssl Poco::NetSSL Poco::Crypto Poco::Util Poco::Net Poco::XML Poco::Foundation )
