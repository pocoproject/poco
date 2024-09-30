add_executable(Net-mail-parser-fuzzer MailParse.cpp)
target_link_libraries(Net-mail-parser-fuzzer PUBLIC Poco::Net)
set_target_properties(Net-mail-parser-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

add_executable(Net-http-parser-fuzzer HTTPParse.cpp)
target_link_libraries(Net-http-parser-fuzzer PUBLIC Poco::Net)
set_target_properties(Net-http-parser-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

