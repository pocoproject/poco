add_executable(download-ssl src/download.cpp)
target_link_libraries(download-ssl Poco::NetSSL Poco::Util Poco::XML)
