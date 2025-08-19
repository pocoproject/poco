add_executable(HTTPReactorTimeServer src/HTTPReactorTimeServer.cpp)
target_link_libraries(HTTPReactorTimeServer PUBLIC Poco::Net Poco::Util Poco::JSON)
