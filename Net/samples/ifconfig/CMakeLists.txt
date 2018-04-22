add_executable(ifconfig src/ifconfig.cpp)
target_link_libraries(ifconfig Poco::Net)
