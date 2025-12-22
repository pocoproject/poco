add_executable(SampleApp src/SampleApp.cpp)

target_link_libraries(SampleApp PUBLIC Poco::Util Poco::JSON Poco::XML)
