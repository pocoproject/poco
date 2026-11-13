poco_add_executable(Data-session-fuzzer DataSession.cpp)
target_link_libraries(Data-session-fuzzer PUBLIC Poco::DataSQLite)
set_target_properties(Data-session-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
