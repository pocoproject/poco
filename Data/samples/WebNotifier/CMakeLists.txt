add_executable(WebNotifier src/WebNotifier.cpp)
target_link_libraries(WebNotifier Poco::DataSQLite Poco::Net)
