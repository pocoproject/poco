add_executable(pocossh src/pocossh.cpp)
target_link_libraries(pocossh Poco::SSH Poco::Foundation ssh)
