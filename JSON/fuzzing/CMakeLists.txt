poco_add_executable(JSON-parse-fuzzer JsonParse.cpp)
target_link_libraries(JSON-parse-fuzzer PUBLIC Poco::JSON)
set_target_properties(JSON-parse-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

poco_add_executable(JSON-var-fuzzer JSONVar.cpp)
target_link_libraries(JSON-var-fuzzer PUBLIC Poco::JSON)
set_target_properties(JSON-var-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

