poco_add_executable(JSON-parse-fuzzer JsonParse.cpp)
target_link_libraries(JSON-parse-fuzzer PUBLIC Poco::JSON)
set_target_properties(JSON-parse-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

poco_add_executable(JSON-var-fuzzer VarFuzz.cpp)
target_link_libraries(JSON-var-fuzzer PUBLIC Poco::JSON)
set_target_properties(JSON-var-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

poco_add_executable(JSON-stringify-fuzzer StringifyFuzz.cpp)
target_link_libraries(JSON-stringify-fuzzer PUBLIC Poco::JSON)
set_target_properties(JSON-stringify-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
