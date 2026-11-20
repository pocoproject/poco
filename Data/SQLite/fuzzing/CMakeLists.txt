poco_add_executable(DataSQLite-sql-fuzzer SQLFuzz.cpp)
target_link_libraries(DataSQLite-sql-fuzzer PUBLIC Poco::DataSQLite)
target_include_directories(DataSQLite-sql-fuzzer PRIVATE
	${PROJECT_SOURCE_DIR}/dependencies/sqlite3/src)
set_target_properties(DataSQLite-sql-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
