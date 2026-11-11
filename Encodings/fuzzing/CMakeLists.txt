poco_add_executable(Encodings-convert-fuzzer EncodingsConvert.cpp)
target_link_libraries(Encodings-convert-fuzzer PUBLIC Poco::Encodings)
set_target_properties(Encodings-convert-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
