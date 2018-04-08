add_executable(dict src/dict.cpp)
target_link_libraries(dict Poco::Net Poco::Util Poco::JSON Poco::XML Poco::Foundation )
