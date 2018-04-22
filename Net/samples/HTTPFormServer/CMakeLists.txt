add_executable(HTTPFormServer src/HTTPFormServer.cpp)
target_link_libraries(HTTPFormServer Poco::Net Poco::Util Poco::JSON)
