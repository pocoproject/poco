poco_add_executable(AIMCPHTTP-eventstream-fuzzer EventStreamFuzz.cpp)
target_link_libraries(AIMCPHTTP-eventstream-fuzzer PUBLIC Poco::AIMCPHTTP)
set_target_properties(AIMCPHTTP-eventstream-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
