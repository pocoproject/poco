add_executable(dict src/dict.cpp)
target_link_libraries(dict PUBLIC Poco::Net Poco::Util Poco::JSON)
