add_executable(DBLogger src/DBLogger.cpp)
target_link_libraries(DBLogger PUBLIC Poco::Util Poco::DataSQLite)
