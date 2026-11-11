poco_add_executable(CppParser-attributes-fuzzer AttributesFuzz.cpp)
target_link_libraries(CppParser-attributes-fuzzer PUBLIC Poco::CppParser)
set_target_properties(CppParser-attributes-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
