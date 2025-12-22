add_executable(JSON-parse-fuzzer JsonParse.cpp)
target_link_libraries(JSON-parse-fuzzer PUBLIC Poco::JSON)
set_target_properties(JSON-parse-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
