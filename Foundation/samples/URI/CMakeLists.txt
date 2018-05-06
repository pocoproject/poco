add_executable(URI src/URI.cpp)
target_link_libraries(URI PUBLIC Poco::JSON Poco::XML Poco::Foundation )
