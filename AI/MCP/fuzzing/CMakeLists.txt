poco_add_executable(AIMCP-server-fuzzer ServerFuzz.cpp)
target_link_libraries(AIMCP-server-fuzzer PUBLIC Poco::AIMCP)
set_target_properties(AIMCP-server-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

poco_add_executable(AIMCP-client-fuzzer ClientFuzz.cpp)
target_link_libraries(AIMCP-client-fuzzer PUBLIC Poco::AIMCP)
set_target_properties(AIMCP-client-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
