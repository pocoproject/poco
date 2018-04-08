add_executable(HTTPLoadTest src/HTTPLoadTest.cpp)
target_link_libraries(HTTPLoadTest Poco::Net Poco::Util Poco::JSON Poco::XML Poco::Foundation)
