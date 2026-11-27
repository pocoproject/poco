poco_add_executable(Encodings-decode-fuzzer DecodeFuzz.cpp)
target_link_libraries(Encodings-decode-fuzzer PUBLIC Poco::Encodings)
set_target_properties(Encodings-decode-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

poco_add_executable(Encodings-encode-fuzzer EncodeFuzz.cpp)
target_link_libraries(Encodings-encode-fuzzer PUBLIC Poco::Encodings)
set_target_properties(Encodings-encode-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
