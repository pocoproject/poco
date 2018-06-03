add_executable(HTTPLoadTest src/HTTPLoadTest.cpp)
target_link_libraries(HTTPLoadTest PUBLIC Poco::Net Poco::Util Poco::JSON)
