add_executable(Foundation-datetime-fuzzer DateTimeParse.cpp)
target_link_libraries(Foundation-datetime-fuzzer PUBLIC Poco::Foundation)
set_target_properties(Foundation-datetime-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

add_executable(Foundation-misc-fuzzer MiscFuzzing.cpp)
target_link_libraries(Foundation-misc-fuzzer PUBLIC Poco::Foundation)
set_target_properties(Foundation-misc-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
