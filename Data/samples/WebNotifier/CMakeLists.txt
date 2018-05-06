add_executable(WebNotifier src/WebNotifier.cpp)
target_link_libraries(WebNotifier PUBLIC Poco::DataSQLite Poco::Net)
