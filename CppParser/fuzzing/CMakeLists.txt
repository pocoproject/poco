poco_add_executable(CppParser-fuzzer CppParserFuzz.cpp)
target_link_libraries(CppParser-fuzzer PUBLIC Poco::CppParser)
set_target_properties(CppParser-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
