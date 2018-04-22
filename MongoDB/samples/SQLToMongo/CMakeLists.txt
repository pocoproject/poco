add_executable(SQLToMongo src/SQLToMongo.cpp)
target_link_libraries(SQLToMongo Poco::MongoDB)
