poco_add_executable(MemoryDB src/MemoryDB.cpp)
target_link_libraries(MemoryDB PUBLIC Poco::DataSQLite)
