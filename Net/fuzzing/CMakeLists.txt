poco_add_executable(Net-mail-parser-fuzzer MailParse.cpp)
target_link_libraries(Net-mail-parser-fuzzer PUBLIC Poco::Net)
set_target_properties(Net-mail-parser-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

poco_add_executable(Net-http-parser-fuzzer HTTPParse.cpp)
target_link_libraries(Net-http-parser-fuzzer PUBLIC Poco::Net)
set_target_properties(Net-http-parser-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

poco_add_executable(Net-address-fuzzer NetAddressCookie.cpp)
target_link_libraries(Net-address-fuzzer PUBLIC Poco::Net)
set_target_properties(Net-address-fuzzer PROPERTIES LINK_FLAGS $ENV{LIB_FUZZING_ENGINE})

