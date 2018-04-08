add_executable(StringTokenizer src/StringTokenizer.cpp)
target_link_libraries(StringTokenizer Poco::JSON Poco::XML Poco::Foundation)
