add_executable(Binding src/Binding.cpp)
target_link_libraries(Binding PUBLIC Poco::DataSQLite)
