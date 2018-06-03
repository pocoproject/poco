add_executable(Ping src/Ping.cpp)
target_link_libraries(Ping PUBLIC Poco::Net Poco::Util Poco::JSON)
