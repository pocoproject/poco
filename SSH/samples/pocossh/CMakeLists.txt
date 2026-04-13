add_executable(pocossh src/pocossh.cpp)
target_link_libraries(pocossh Poco::SSH Poco::Foundation)
target_include_directories(pocossh PRIVATE ${LIBSSH_INCLUDE_DIRS})
