add_executable(download src/download.cpp)
target_link_libraries(download PUBLIC Poco::Net Poco::Util Poco::JSON)
