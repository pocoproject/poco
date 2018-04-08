add_executable(TypeHandler src/TypeHandler.cpp)
target_link_libraries(TypeHandler Poco::DataSQLite Poco::Data Poco::Foundation )
