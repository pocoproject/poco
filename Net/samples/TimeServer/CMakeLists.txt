add_executable(TimeServer src/TimeServer.cpp)
target_link_libraries(TimeServer PUBLIC Poco::Net Poco::Util Poco::JSON)
