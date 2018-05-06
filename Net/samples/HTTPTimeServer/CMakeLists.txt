add_executable(HTTPTimeServer src/HTTPTimeServer.cpp)
target_link_libraries(HTTPTimeServer PUBLIC Poco::Net Poco::Util Poco::JSON)
