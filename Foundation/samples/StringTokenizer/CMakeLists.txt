add_executable(StringTokenizer src/StringTokenizer.cpp)
target_link_libraries(StringTokenizer PUBLIC Poco::JSON Poco::XML Poco::Foundation)
