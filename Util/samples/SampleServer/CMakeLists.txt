add_executable(SampleServer src/SampleServer.cpp)

target_link_libraries(SampleServer PUBLIC Poco::Util Poco::JSON Poco::XML)
