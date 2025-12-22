add_executable(XML-parse-fuzzer XMLParse.cpp)
target_link_libraries(XML-parse-fuzzer PUBLIC Poco::XML)
set_target_properties(XML-parse-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})
