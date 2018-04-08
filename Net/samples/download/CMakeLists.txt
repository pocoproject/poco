add_executable(download src/download.cpp)
target_link_libraries(download Poco::Net Poco::Util Poco::JSON Poco::XML Poco::Foundation)
