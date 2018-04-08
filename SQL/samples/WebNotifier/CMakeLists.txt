add_executable(WebNotifier src/WebNotifier.cpp)
target_link_libraries(WebNotifier Poco::SQLSQLite Poco::Net)
