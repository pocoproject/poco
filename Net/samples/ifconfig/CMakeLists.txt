add_executable(ifconfig src/ifconfig.cpp)
target_link_libraries(ifconfig Poco::Net Poco::Util Poco::JSON Poco::XML Poco::Foundation )
