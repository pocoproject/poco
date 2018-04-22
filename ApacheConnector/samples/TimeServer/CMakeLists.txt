add_library(TimeServer-apache src/TimeServer.cpp)
target_link_libraries(TimeServer-apache Poco::Net)
