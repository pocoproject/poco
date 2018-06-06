add_executable(Mail src/Mail.cpp)
target_link_libraries(Mail PUBLIC Poco::Net Poco::Util Poco::JSON)
