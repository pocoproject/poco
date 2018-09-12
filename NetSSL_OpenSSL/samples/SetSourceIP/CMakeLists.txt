add_executable(SetSourceIP src/SetSourceIP.cpp)
target_link_libraries(SetSourceIP PUBLIC Poco::NetSSL Poco::Crypto Poco::Util Poco::Net Poco::XML Poco::Foundation)
# uncomment following line, might solve compiling issues
#set(CMAKE_CXX_STANDARD 11)

