add_executable(HTTPFormServer src/HTTPFormServer.cpp)
target_link_libraries(HTTPFormServer PUBLIC Poco::Net Poco::Util Poco::JSON)
