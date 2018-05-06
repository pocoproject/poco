add_executable(Mail-ssl src/Mail.cpp)
target_link_libraries(Mail-ssl PUBLIC Poco::NetSSL Poco::Util Poco::XML)
