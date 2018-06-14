add_executable(ifconfig src/ifconfig.cpp)
target_link_libraries(ifconfig PUBLIC Poco::Net)
