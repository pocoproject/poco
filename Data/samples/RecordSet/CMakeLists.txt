add_executable(RecordSet src/RecordSet.cpp)
target_link_libraries(RecordSet PUBLIC Poco::DataSQLite)
