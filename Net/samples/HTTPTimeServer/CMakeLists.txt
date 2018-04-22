add_executable(HTTPTimeServer src/HTTPTimeServer.cpp)
target_link_libraries(HTTPTimeServer Poco::Net Poco::Util Poco::JSON)
