add_executable(TimeServer src/TimeServer.cpp)
target_link_libraries(TimeServer Poco::Net Poco::Util Poco::JSON Poco::XML Poco::Foundation)
