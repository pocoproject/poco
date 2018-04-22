add_executable(Mail-ssl src/Mail.cpp)
target_link_libraries(Mail-ssl Poco::NetSSL Poco::Util Poco::XML)
