add_executable(Ping src/Ping.cpp)
target_link_libraries(Ping Poco::Net Poco::Util Poco::JSON Poco::XML Poco::Foundation)
